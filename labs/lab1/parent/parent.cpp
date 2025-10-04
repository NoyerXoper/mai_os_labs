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

    //os::Descriptor fd = os::OpenReadFile(path);
    os::Descriptor fd = os::OpenReadFile(path);
    if (fd == -1) {
        printf("file didn't open");
        return 1;
    }

    if(os::CreatePipe(pipe) == -1) {
        printf("pipe didn't open");
        os::CloseDescriptor(fd);
        return 1;
    } 

    os::Process child("./build/labs/lab1/child", pipe);
    child.SetPipeMode(CLOSE_READ_PIPE);
    child.stdInDescriptor = fd;
    child.stdOutDescriptor = pipe[1];

    child.Start();
    
    // right here we are in parent, child is not here anymore
    os::CloseDescriptor(fd);
    os::CloseDescriptor(pipe[1]);

    ssize_t bytes_read;
    char buffer[BUFFER_SIZE];
    printf("First positive composite numbers in file:\n");
    while ((bytes_read = os::ReadToBuffer(pipe[0], buffer, sizeof(buffer)-1)) > 0) {
        buffer[bytes_read] = '\0';
        printf(buffer);
        fflush(stdout);
    }
    os::CloseDescriptor(pipe[0]);
    os::Wait(&status);
    return 0;
}
