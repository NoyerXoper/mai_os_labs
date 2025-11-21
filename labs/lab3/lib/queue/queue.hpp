#pragma once

#include <array>
#include <string_view>

#include "os.hpp"

namespace shared_queue {
namespace {
template <class T, std::size_t size>
struct QueueWrapper {
public:
    std::size_t in = 0;
    std::size_t out = 0;
    std::size_t producer_count = 0;
    bool finished = false;
    alignas(alignof(T)) std::byte data[sizeof(T) * size];

    inline T* GetPtrOfOffset(std::size_t offset) noexcept {
        return reinterpret_cast<T*>(data + offset * sizeof(T));
    }
    
    inline const T* GetPtrOfOffset(std::size_t offset) const noexcept {
        return reinterpret_cast<T*>(data + offset * sizeof(T));
    }

    inline T* GetFrontPtr() noexcept {
        return GetPtrOfOffset(out);
    }

    inline const T* GetFrontPtr() const noexcept {
        return GetPtrOfOffset(out);
    }

    inline T* GetBackPtr() noexcept {
        return GetPtrOfOffset(in);
    }

    inline const T* GetBackPtr() const noexcept {
        return GetPtrOfOffset(in);
    }
};
}
template<class T, std::size_t size, bool mode>
class NamedSharedQueue {
public:
    NamedSharedQueue(std::string_view name, bool create=true);
    void PushBack(const T& obj) requires mode;
    void PopFront() requires (!mode);
    bool AreProducersDead() noexcept requires (!mode);
    bool AreProducersWorking() noexcept requires (!mode);
    std::size_t Size() noexcept;
    T Front() noexcept;
    ~NamedSharedQueue() noexcept;
private:
    os::SharedObject<QueueWrapper<T, size>> data_;
    os::NamedSemaphore empty;
    os::NamedSemaphore full;
    os::NamedSemaphore mutex;
};

template<class T, std::size_t size>
using ReadingEndSharedQueue = NamedSharedQueue<T, size, false>;

template<class T, std::size_t size>
using WritingEndSharedQueue = NamedSharedQueue<T, size, true>;
}

#include "queue.ipp"
