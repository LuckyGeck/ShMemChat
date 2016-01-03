#pragma once


namespace NSem {
/// Wrapper for working with semaphore
class TSem {
public:
    /// @param own - if it's true, object will own semaphore and destroy it in dtor
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
