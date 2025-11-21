#include <string>
#include <stdio.h>
#include <string.h>

#include "os.hpp"
#include "queue.hpp"

int main() {
    os::Descriptor pipe_descriptors[2];
    std::string path;
    int status;

    printf("Enter the path to the file:");
    std::getline(std::cin, path);

    os::Descriptor fd = os::OpenReadFile(path);
    std::string_view shared_memory_name = "lab3_shared_memory";

    shared_queue::ReadingEndSharedQueue<int, 512> shared_memory(shared_memory_name);
    os::Process child("child");
    child.AddArgv(shared_memory_name);
    child.stdInDescriptor = fd;

    child.Start();
    os::ProcessID processId = child.GetId();
    
    // right here we are in parent, child is not here anymore
    os::CloseDescriptor(fd);

    printf("First positive composite numbers in file:\n");
    while(shared_memory.AreProducersWorking() || shared_memory.Size() > 0) {
        std::cout << shared_memory.Front() << '\n';
        shared_memory.PopFront();
    }
    os::Wait(&status, processId);
    return 0;
}
