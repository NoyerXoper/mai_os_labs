namespace os {

using void_func = typeof(void*(void*))*;

struct ThreadInfo;
using ThreadHandle = ThreadInfo*;

class Thread {
public:
    Thread(void_func func);

    Thread(const Thread&) = delete;
    Thread(Thread&& other) noexcept;
    Thread& operator=(const Thread&) = delete;
    Thread& operator=(Thread&& other) noexcept;

    void Swap(Thread& other);

    void Run(void* data);
    void Join();

    ~Thread() noexcept;
private:
    void_func function_;
    ThreadHandle handle_;
};
}
