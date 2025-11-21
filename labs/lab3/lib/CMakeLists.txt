include_directories(queue "system" exceptions)
# Multiplatform in ifdef in the end of os.hpp

add_library(exceptions INTERFACE)
target_link_directories(exceptions INTERFACE exceptions)

add_library(sys INTERFACE)
target_link_directories(sys INTERFACE "system")
target_link_libraries(sys INTERFACE exceptions sys)

add_library(shared_queue INTERFACE)
target_link_directories(shared_queue INTERFACE queue)
target_link_libraries(shared_queue INTERFACE sys)
target_link_libraries(shared_queue INTERFACE exceptions sys)
