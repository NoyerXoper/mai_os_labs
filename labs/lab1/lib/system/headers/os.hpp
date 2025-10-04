#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace os {
#define BUFFER_SIZE 1024
#define CLOSE_READ_PIPE 1
#define CLOSE_WRITE_PIPE 2

typedef int Descriptor;
typedef int Pipe[2];

struct Process {
public:
    Process(std::string path, Pipe pipe);
    void SetPipeMode(int mode);
    void AddArgv(const std::string& arg);
    char** GetArgv();
    void Start();
    virtual ~Process() noexcept;
private:
    std::string path_;
    Pipe pipe_;
    int mode_;
    std::vector<char*> argv_;
public:
    Descriptor stdInDescriptor = -1;
    Descriptor stdOutDescriptor = -1;

};

//void CreateProcess(const char* path, Pipe pipe, int mode, std::size_t argc, char* const argv[]);

int CreatePipe(Pipe fds);

void WriteToDescriptor(Descriptor d, std::string data);

int ReadToBuffer(Descriptor d, char buffer[], std::size_t size);

void CloseDescriptor(Descriptor d);

Descriptor OpenReadFile(const char* file_path);

void Wait(int* status);

bool DidProcessEndSuccessfully(int status);
}