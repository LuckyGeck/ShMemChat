#pragma once


namespace NSem {
class TSem {
public:
    TSem(const char* path, bool own);
    ~TSem();

    void Inc(int count = 1) {
        Op(count);
    }
    void Dec(int count = 1) {
        Op(-count);
    }

private:
    void Op(int sem_op);

private:
    bool Owner;
    int SemId;
};
} // namespace NSem
