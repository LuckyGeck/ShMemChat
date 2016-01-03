#include "shq.h"
#include "sem.h"

#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>


namespace {
struct TShmemHead {
    size_t TotalMsgsLength;

    void Clear() {
        TotalMsgsLength = 0;
    }

    void AddMsg(const char* msg) {
        strcpy((char*)this + sizeof(TotalMsgsLength) + TotalMsgsLength, msg);
        TotalMsgsLength += strlen(msg) + 1;
    }

    size_t IterateMsgs(NShQ::TShQ::TProcessingFn& fn) const {
        const char* beginning = (const char*)this + sizeof(TotalMsgsLength);
        size_t curOffset = 0;
        size_t msgRead = 0;
        while (curOffset < TotalMsgsLength) {
            size_t curLen = strlen(beginning + curOffset);
            fn(beginning + curOffset);
            ++msgRead;
            curOffset += curLen + 1;
        }
        return msgRead;
    }
};
} // anonymous namespace

namespace NShQ {

class TShQ::TImpl {
public:
    TImpl(const char* path, size_t shmemSize, bool ownQueue)
        : OwnQueue(ownQueue)
        , Key(ftok(path, 'R'))
        , ShMemId(shmget(Key, shmemSize, 0777 | IPC_CREAT))
        , ShMemSize(shmemSize)
        , ShMem((TShmemHead*)shmat(ShMemId, nullptr, 0))
        , WriteSem(path, ownQueue)
    {
        if (OwnQueue) {
            WriteSem.Inc();
        }
    }

    ~TImpl() {
        shmdt((void*)ShMem);
        if (OwnQueue) {
            shmctl(ShMemId, IPC_RMID, NULL);
        }
    }

    void AppendMsg(const char* msg) {
        Lock();
        ShMem->AddMsg(msg);
        Unlock();
    }

    size_t ProcessAndClean(TShQ::TProcessingFn fn) {
        Lock();
        size_t messagesCount = ShMem->IterateMsgs(fn);
        ShMem->Clear();
        Unlock();
        return messagesCount;
    }

private:
    void Lock() {
        WriteSem.Dec();
    }

    void Unlock() {
        WriteSem.Inc();
    }

private:
    bool OwnQueue;
    key_t Key;
    int ShMemId;
    size_t ShMemSize;
    TShmemHead* ShMem;
    NSem::TSem WriteSem;
};

TShQ::TShQ(const char* path, size_t shmemSize, bool ownQueue)
    : Impl(new TImpl(path, shmemSize, ownQueue))
{}

TShQ::~TShQ() = default;

void TShQ::AppendMsg(const char* msg) {
    Impl->AppendMsg(msg);
}

size_t TShQ::ProcessAndClean(TShQ::TProcessingFn fn) {
    return Impl->ProcessAndClean(fn);
}

} // namespace NShQ
