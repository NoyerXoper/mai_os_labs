#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "os.hpp"

namespace os
{

namespace {
char buffer[BUFFER_SIZE];
}

struct FileHandle {
    int descriptor;
};
struct ProcessHandle {
    pid_t id;
};

PipeHandle::PipeHandle(): status_(false) {
    fd[0] = new FileHandle();    
    fd[1] = new FileHandle();    
}

void PipeHandle::Open() {
    int fdUNIX[2];
    status_ = pipe(fdUNIX) != -1;
    fd[0]->descriptor = fdUNIX[0];
    fd[1]->descriptor = fdUNIX[1];
}

HANDLE& PipeHandle::GetReadPipe() {
    return fd[0];
}

HANDLE& PipeHandle::GetWritePipe() {
    return fd[1];
}

void PipeHandle::Write(const std::string& data) {
    write(GetWritePipe()->descriptor, data.c_str(), data.size());
}

std::string Pipe::Read() {
    int bytes_read = read(GetReadPipe()->descriptor, buffer, BUFFER_SIZE);
    return std::string(buffer, buffer + bytes_read);
}
void PipeHandle::CloseRead() {
    if(!GetReadPipe()) return;
    close(GetReadPipe()->descriptor);
    delete fd[0];
    fd[0] = nullptr;
}

void PipeHandle::CloseWrite() {
    if(!GetWritePipe()) return;
    close(GetWritePipe()->descriptor);
    delete fd[1];
    fd[1] = nullptr;
}

bool PipeHandle::GetStatus() {
    return status_;
}

PipeHandle::~PipeHandle() noexcept {
    delete fd[0];
    delete fd[1];
}

Process::Process(std::string path, Pipe& pipe): path_(path), pipe_(pipe), mode_(0) {}

void Process::SetPipeMode(int mode) {
    mode_ = mode;
}

void Process::AddArgv(const std::string& arg) {
    char* data = new char[arg.size() + 1];
    strcpy(data, arg.c_str());
    argv_.push_back(data);
}

char** Process::GetArgv() {
    char** data = new char*[argv_.size() + 2];
    memcpy(data+1, argv_.data(), argv_.size() * sizeof(char*));
    data[0] = path_.data();
    data[argv_.size() + 1] = NULL;
    return data;
}

Process::~Process() noexcept {
    for (char*& arg: argv_) {
        delete[] arg;
    }
    delete processId_;
}

ProcessID Process::GetId() const noexcept {
    return processId_;
}

void Process::Start() {
    if (processId_) {
        delete processId_;
    }
    pid_t id = fork();
    if (id == 0) {
        if (mode_ & CLOSE_READ_PIPE) {
            pipe_.CloseRead();
        }
        if (mode_ & CLOSE_WRITE_PIPE) {
            pipe_.CloseWrite();
        }
        if (stdInDescriptor) {
            dup2(stdInDescriptor->descriptor, STDIN_FILENO);

        }
        if (stdOutDescriptor) {
            dup2(stdOutDescriptor->descriptor, STDOUT_FILENO);
        }
        pipe_.~PipeHandle();
        execv(path_.c_str(), GetArgv());
    } else if (id > 0) {
        processId_ = new ProcessHandle();
        processId_->id = id;
    }
}

int CreatePipe(Pipe& fd) {
    int fdUNIX[2];
    int res = pipe(fdUNIX);
    fd.GetReadPipe()->descriptor = fdUNIX[0];
    fd.GetWritePipe()->descriptor = fdUNIX[1];
    return res;
}


void CloseDescriptor(Descriptor& d) {
    close(d->descriptor);
    delete d;
}

Descriptor OpenReadFile(const char* path) {
    try {
        int descriptor = open(path, O_RDONLY);
        if (descriptor == -1) {
            return nullptr;
        }
        Descriptor de = new FileHandle();
        de->descriptor = descriptor;
        return de;
    } catch(const std::bad_alloc& exc) {
        return nullptr;
    }
}

void Wait(int* status, ProcessID id) {
    while (id->id != wait(status)) {};
}

bool DidProcessEndSuccessfully(int status) {
    return WIFEXITED(status);
}
}

