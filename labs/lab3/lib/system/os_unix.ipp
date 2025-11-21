#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <iostream>

#include "os.hpp"
#include "exceptions.hpp"

namespace os
{
struct FileDummy {
    int descriptor;
};

struct ProcessHandle {
    pid_t id;
};

struct SemaphoreDummy {
    sem_t* sem_id;
};

template<class T>
SharedObject<T>::SharedObject(const T& obj, std::string_view name, int mode, bool do_create) requires std::is_copy_constructible_v<T>: 
                                        name_(name), is_owner_(do_create) {
    int posix_desc_sh_mode = do_create ? O_CREAT : 0;
    int prot = PROT_READ;
    if (do_create || mode == WriteReadSharedObject)  {
        posix_desc_sh_mode |= O_RDWR;
        prot |= PROT_WRITE;
    } else if (mode == ReadOnlySharedObject) {
        posix_desc_sh_mode |= O_RDONLY;
    }

    int shd_mem_desc = shm_open(name_.c_str(), posix_desc_sh_mode, 0666);
    if (shd_mem_desc == -1) {
        throw exceptions::SystemError(errno, "shm_open error");
    }
    if (do_create && ftruncate(shd_mem_desc, sizeof(T)) == -1) {
        if (do_create) {
            if (shm_unlink(name_.c_str()) == -1) {
                std::cerr << "Warning: after ftruncate failed, shm_unlink also failed, maybe it leaked.\n";
            }
        }
        throw exceptions::SystemError(errno, "ftruncate error");
    }
    void* buffer = mmap(NULL, sizeof(T), prot, MAP_SHARED, shd_mem_desc, 0);
    close(shd_mem_desc);
    if (buffer == MAP_FAILED) {
        if (do_create) {
            if (shm_unlink(name_.c_str()) == -1) {
                std::cerr << "Warning: after mmap failed, shm_unlink also failed, maybe it leaked.\n";
            }
        }
        throw exceptions::SystemError(errno, "mmap failed");
    }
    
    // It can throw anything.
    object_ = reinterpret_cast<T*>(buffer);
    try {
        if (do_create) {
            std::construct_at(object_, obj);
        }
    } catch (const std::exception&) {
        munmap(object_, sizeof(T));
        if (do_create) {
            if (shm_unlink(name_.c_str()) == -1) {
                std::cerr << "Warning: after constructor of T throwed, shm_unlink also failed, maybe it leaked.\n";
            }
        }
        throw;
    }
}

template<class T>
SharedObject<T>::SharedObject(std::string_view name, int mode, bool do_create) requires std::is_default_constructible_v<T>: 
                                        SharedObject(T(), name, mode, do_create) {}

template<class T>
T& SharedObject<T>::operator*() noexcept {
    return *object_;
}

template<class T>
const T& SharedObject<T>::operator*() const noexcept {
    return *object_;
}

template<class T>
T* SharedObject<T>::operator->() noexcept {
    return object_;
}

template<class T>
const T* SharedObject<T>::operator->() const noexcept {
    return object_;
}

template<class T>
const std::string& SharedObject<T>::GetName() const noexcept {
    return name_;
}

template<class T>
SharedObject<T>::~SharedObject() noexcept {
    munmap(object_, sizeof(T));
    if (is_owner_) {
        shm_unlink(name_.c_str());
    }
}

NamedSemaphore::NamedSemaphore() noexcept: semaphore_(nullptr) {}

NamedSemaphore::NamedSemaphore(std::string_view name, unsigned int initial_counter): name_(name) {
    semaphore_ = new SemaphoreDummy;
    semaphore_->sem_id = sem_open(name_.c_str(), O_CREAT, 0666, initial_counter);
}

NamedSemaphore::NamedSemaphore(NamedSemaphore&& other) noexcept: name_(std::move(other.name_)), semaphore_(other.semaphore_) {
    other.semaphore_ = nullptr;
}

NamedSemaphore& NamedSemaphore::operator=(NamedSemaphore&& other) noexcept {
    NamedSemaphore temp = std::move(other);
    std::swap(name_, temp.name_);
    std::swap(semaphore_, temp.semaphore_);
    return *this;
}

void NamedSemaphore::Wait() {
    if (semaphore_) {
        sem_wait(semaphore_->sem_id);
    }
}

void NamedSemaphore::Post() {
    if (semaphore_) {
        sem_post(semaphore_->sem_id);
    }
}

int NamedSemaphore::GetValue() const noexcept {
    if (semaphore_) {
        int val;
        sem_getvalue(semaphore_->sem_id, &val);
        return val;
    }
    return 0;
}

NamedSemaphore::~NamedSemaphore() noexcept {
    if (semaphore_) {
        sem_unlink(name_.c_str());
    }
    delete semaphore_;
}

LockSemGuard::LockSemGuard(NamedSemaphore& sem): sem_(sem) {
    sem_.Wait();
}

LockSemGuard::~LockSemGuard() {
    sem_.Post();
}

Process::Process(std::string_view path): path_(path) {}

void Process::AddArgv(std::string_view arg) {
    argv_.emplace_back(arg);
}

char** Process::GetArgv() {
    char** data = new char*[argv_.size() + 2];
    for (std::size_t i = 1; i <= argv_.size(); ++i) {
        data[i] = argv_[i - 1].data();
    }
    data[0] = path_.data();
    data[argv_.size() + 1] = NULL;
    return data;
}

ProcessID Process::GetId() const noexcept {
    return processId_;
}

void Process::Start() {
    if (processId_) {
        throw exceptions::DoubleStartProcessError("Can't start process twice");
    }
    pid_t id = fork();
    if (id == 0) {
        if (stdInDescriptor) {
            dup2(stdInDescriptor->descriptor, STDIN_FILENO);
        }
        if (stdOutDescriptor) {
            dup2(stdOutDescriptor->descriptor, STDOUT_FILENO);
        }
        execv(path_.c_str(), GetArgv());
    } else if (id > 0) {
        processId_ = new ProcessHandle();
        processId_->id = id;
    }
    else {
        throw exceptions::SystemError(errno, "Couldn't fork the process");
    }
}

Process::~Process() noexcept {
    delete processId_;
}

Descriptor OpenReadFile(std::string_view path) {
    int descriptor = open(path.data(), O_RDONLY);
    if (descriptor == -1) {
        throw exceptions::SystemError(errno, "Couldn't open file");
    }
    Descriptor de = new FileDummy();
    de->descriptor = descriptor;
    return de;
}

void CloseDescriptor(Descriptor desc) {
    if (desc) {
        close(desc->descriptor);
    }
    delete desc;
}

void Wait(int* status, ProcessID id) {
    waitpid(id->id, status, 0);
}
}
