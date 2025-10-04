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

Process::Process(std::string path, Pipe pipe): path_(path), mode_(0) {
    pipe_[0] = pipe[0];
    pipe_[1] = pipe[1];
}

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
}

void Process::Start() {
    pid_t id = fork();
    if (id == 0) {
        if (mode_ & CLOSE_READ_PIPE) {
            CloseDescriptor(pipe_[0]);
        }
        if (mode_ & CLOSE_WRITE_PIPE) {
            CloseDescriptor(pipe_[1]);
        }
        if (stdInDescriptor != -1) {
            dup2(stdInDescriptor, STDIN_FILENO);

        }
        if (stdOutDescriptor != -1) {
            dup2(stdOutDescriptor, STDOUT_FILENO);
        }
        execv(path_.c_str(), GetArgv());
    }
}

int CreatePipe(Pipe fd) {
    return pipe(fd);
}

void WriteToDescriptor(Descriptor d, std::string data) {
    write(d, data.c_str(), data.size());
}

int ReadToBuffer(Descriptor d, char buffer[], std::size_t size) {
    return read(d, buffer, size);
}

void CloseDescriptor(Descriptor d) {
    close(d);
}

Descriptor OpenReadFile(const char* path) {
    return open(path, O_RDONLY);
}

void Wait(int* status) {
    wait(status);
}

bool DidProcessEndSuccessfully(int status) {
    return WIFEXITED(status);
}
}

