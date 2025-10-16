#include <pthread.h>
#include <utility>
#include <system_error>

#include "threads.hpp"


namespace os {
struct ThreadInfo {
    pthread_t thread;
};

Thread::Thread(void_func func): function_(func) {
    handle_ = new ThreadInfo();
}

Thread::Thread(Thread&& other) noexcept: function_(other.function_), handle_(other.handle_) {
    other.function_ = nullptr;
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

void Thread::Run(void* data) {
    int res = pthread_create(&(handle_->thread), NULL, function_, data);
    if (res != 0) {
        throw std::system_error(res, std::system_category(), "pthread_create failed");
    }
}

void Thread::Join() {
    int res = pthread_join(handle_->thread, NULL);
    if (res != 0) {
        throw std::system_error(res, std::system_category(), "pthread_join failed");
    }
}

void Thread::Swap(Thread& other) {
    std::swap(function_, other.function_);
    std::swap(handle_, other.handle_);
}
}
