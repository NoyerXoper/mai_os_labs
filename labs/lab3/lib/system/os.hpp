#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include <memory>

namespace os {
constexpr int ReadOnlySharedObject = 1;
constexpr int WriteReadSharedObject = 3;

struct FileDummy;
typedef FileDummy* Descriptor;

struct ProcessHandle;
typedef ProcessHandle* ProcessID;

struct SemaphoreDummy;
typedef SemaphoreDummy* SemaphoreHandle;

template<class T>
class SharedObject {
public:
   SharedObject(const T& obj, std::string_view name, int mode, bool do_create=true) requires std::is_copy_constructible_v<T>; 
   SharedObject(std::string_view name, int mode, bool do_create=true) requires std::is_default_constructible_v<T>;
   T& operator*() noexcept;
   const T& operator*() const noexcept;

   T* operator->() noexcept;
   const T* operator->() const noexcept;

   const std::string& GetName() const noexcept;

   ~SharedObject() noexcept;
private:
    std::string name_;
    T* object_;
    const bool is_owner_;

};

class NamedSemaphore {
public:
    NamedSemaphore() noexcept;
    NamedSemaphore(std::string_view name, unsigned int initial_counter);
    NamedSemaphore(NamedSemaphore&& other) noexcept;

    NamedSemaphore& operator=(NamedSemaphore&& other) noexcept;

    void Wait();
    void Post();

    int GetValue() const noexcept;

    ~NamedSemaphore() noexcept;

private:
    std::string name_;
    SemaphoreHandle semaphore_;
};

class LockSemGuard {
public:
    LockSemGuard(NamedSemaphore& sem);
    ~LockSemGuard();
private:
    NamedSemaphore& sem_;
};

struct Process {
public:
    explicit Process(std::string_view path);
    void AddArgv(std::string_view arg);
    char** GetArgv();
    void Start();
    ProcessID GetId() const noexcept;
    ~Process() noexcept;
private:
    std::string path_;
    std::vector<std::string> argv_;
    ProcessID processId_ = nullptr;
public:
    Descriptor stdInDescriptor = nullptr;
    Descriptor stdOutDescriptor = nullptr;

};

Descriptor OpenReadFile(std::string_view path);
void CloseFile(Descriptor desc);

void Wait(int* status, ProcessID id);

}

#ifdef __linux__
#include "os_unix.ipp"
#elif _WIN32
#include "os_win.ipp"
#endif
