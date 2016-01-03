#include <functional>
#include <memory>

namespace NShQ {

class TShQ {
public:
    using TProcessingFn = std::function<void(const char*)>;

public:
    TShQ(const char* path, size_t shmemSize, bool ownQueue);
    ~TShQ();

    void AppendMsg(const char* msg);
    size_t ProcessAndClean(TProcessingFn fn);

private:
    class TImpl;

private:
    std::unique_ptr<TImpl> Impl;
};

} // namespace NShQ