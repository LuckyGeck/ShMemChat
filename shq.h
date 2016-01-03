#include <functional>
#include <memory>

namespace NShQ {

/// Message Queue, that is based on shared memory
/// Semaphore is used to implement mutex for memory access
class TShQ {
public:
    using TProcessingFn = std::function<void(const char*)>;

public:
    /// @param path should be valid filesystem path to existing file (is passed to ftok)
    /// @param shmemSize sets the size of shared memory
    /// @param ownQueue - if it's true, current object will destroy shmem and sem in dtor
    TShQ(const char* path, size_t shmemSize, bool ownQueue);
    ~TShQ();

    void AppendMsg(const char* msg);
    /// Calls fn for each message in queue and cleans up shmem
    /// @return number of messages, that were passed to fn
    size_t ProcessAndClean(TProcessingFn fn);

private:
    class TImpl;

private:
    std::unique_ptr<TImpl> Impl;
};

} // namespace NShQ