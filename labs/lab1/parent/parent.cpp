#include <string>
#include <stdio.h>
#include <string.h>

#include "os.hpp"

int main() {
    char path[256];
    os::Descriptor pipe_descriptors[2];
    os::Pipe pipe;
    int status;

    printf("Enter the path to the file:");
    scanf("%s", &path);

    os::Descriptor fd = os::OpenReadFile(path);
    if (!fd) {
        printf("file didn't open");
        return 1;
    }

    pipe.Open();
    if(!pipe.GetStatus()) {
        printf("pipe didn't open");
        os::CloseDescriptor(fd);
        return 1;
    } 

    os::Process child("./build/labs/lab1/child", pipe);
    child.SetPipeMode(CLOSE_READ_PIPE);
    child.stdInDescriptor = fd;
    child.stdOutDescriptor = pipe.GetWritePipe();

    child.Start();
    os::ProcessID processId = child.GetId();
    
    // right here we are in parent, child is not here anymore
    os::CloseDescriptor(fd);
    pipe.CloseWrite();

    ssize_t bytes_read;
    char buffer[BUFFER_SIZE];
    printf("First positive composite numbers in file:\n");
    std::string data;
    while ((data = pipe.Read()).size()) {
        printf("%s", data.c_str());
    }
    pipe.CloseRead();
    os::Wait(&status, processId);
    return 0;
}
