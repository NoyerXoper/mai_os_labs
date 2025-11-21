#include "queue.hpp"

namespace shared_queue {
template <class T, std::size_t size, bool mode>
NamedSharedQueue<T, size, mode>::NamedSharedQueue(std::string_view name, bool create): data_(name, mode ? os::WriteReadSharedObject : os::ReadOnlySharedObject, create) {
    const std::string& shm_name = data_.GetName();
    empty = os::NamedSemaphore(shm_name + "_empty_sem", size);
    full = os::NamedSemaphore(shm_name + "_full_sem", 0);
    mutex = os::NamedSemaphore(shm_name + "_mutex", 1);
    if constexpr (mode) {
        mutex.Wait();
        ++data_->producer_count; 
        data_->finished = false;
        mutex.Post();
    }
} 

template <class T, std::size_t size, bool mode>
void NamedSharedQueue<T, size, mode>::PushBack(const T& obj) requires mode {
    empty.Wait();
    mutex.Wait();

    std::construct_at(data_->GetBackPtr(), obj);
    data_->in = (data_->in + 1) % size;

    mutex.Post();
    full.Post();
}

template <class T, std::size_t size, bool mode>
void NamedSharedQueue<T, size, mode>::PopFront() requires (!mode) {
    full.Wait();
    mutex.Wait();

    std::destroy_at(data_->GetFrontPtr());
    data_->out = (data_->out + 1) % size;

    mutex.Post();
    empty.Post();
}

template <class T, std::size_t size, bool mode>
T NamedSharedQueue<T, size, mode>::Front() noexcept {
    os::LockSemGuard lockEmpty(full);
    os::LockSemGuard lockAccess(mutex);
    return *(data_->GetFrontPtr());
}

template <class T, std::size_t size, bool mode>
std::size_t NamedSharedQueue<T, size, mode>::Size() noexcept {
    std::size_t size_;
    mutex.Wait();
    if (data_->in >= data_->out) {
        size_ = data_->in - data_->out;
    } else {
        size_ = size - (data_->out - data_->in);
    }
    mutex.Post();
    return size_;

}

template <class T, std::size_t size, bool mode>
bool NamedSharedQueue<T, size, mode>::AreProducersDead() noexcept requires (!mode){
    return data_->producer_count == 0;
}

template <class T, std::size_t size, bool mode>
bool NamedSharedQueue<T, size, mode>::AreProducersWorking() noexcept requires (!mode) {
    return !data_->finished;
}

template <class T, std::size_t size, bool mode>
NamedSharedQueue<T, size, mode>::~NamedSharedQueue() noexcept {
    if constexpr (mode) {
        if(--data_->producer_count == 0) {
            data_->finished = true;
        }
    }
}
}