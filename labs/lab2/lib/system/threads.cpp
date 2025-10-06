#include <pthread.h>
#include <utility>

#include "threads.hpp"


namespace os {
struct ThreadInfo {
    pthread_t thread;
};

Thread::Thread(void_func func): function_(func) {
    handle_ = new ThreadInfo();
}

Thread::Thread(Thread&& other) noexcept: function_(other.function_), handle_(other.handle_) {
    function_ = nullptr;
    other.handle_ = nullptr;
}

Thread::~Thread() noexcept {
    delete handle_;
}


Thread& Thread::operator=(Thread&& other) noexcept {
    if (this == &other) {
        return *this;
    }
    Thread moved = std::move(other);
    this->Swap(moved);
    return *this;
}

bool Thread::Run(void* data) {
    return pthread_create(&(handle_->thread), NULL, function_, data) == 0;
}

void Thread::Join() {
    pthread_join(handle_->thread, NULL);
}

void Thread::Swap(Thread& other) {
    std::swap(function_, other.function_);
    std::swap(handle_, other.handle_);
}
}