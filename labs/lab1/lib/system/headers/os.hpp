#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace os {
#define BUFFER_SIZE 1024
#define CLOSE_READ_PIPE 1
#define CLOSE_WRITE_PIPE 2

struct FileHandle;
struct ProcessHandle;
typedef FileHandle* HANDLE;
typedef ProcessHandle* ProcessID;
struct PipeHandle {
    PipeHandle();

    void Open();
    HANDLE& GetReadPipe();
    HANDLE& GetWritePipe();
    void CloseRead();
    void CloseWrite();
    bool GetStatus();
    void Write(const std::string& line); 
    std::string Read();
    ~PipeHandle() noexcept;

private:
    bool status_;
    HANDLE fd[2];
};

typedef HANDLE Descriptor;
typedef PipeHandle Pipe;

struct Process {
public:
    Process(std::string path, Pipe& pipe);
    void SetPipeMode(int mode);
    void AddArgv(const std::string& arg);
    char** GetArgv();
    void Start();
    ProcessID GetId() const noexcept;
    virtual ~Process() noexcept;
private:
    std::string path_;
    Pipe& pipe_;
    int mode_;
    std::vector<char*> argv_;
    ProcessID processId_ = nullptr;
public:
    Descriptor stdInDescriptor = nullptr;
    Descriptor stdOutDescriptor = nullptr;

};

void WriteToDescriptor(Descriptor d, std::string data);

int ReadToBuffer(Descriptor d, char buffer[], std::size_t size);

void CloseDescriptor(Descriptor& d);

Descriptor OpenReadFile(const char* file_path);

void Wait(int* status, ProcessID id);

bool DidProcessEndSuccessfully(int status);
}