add_subdirectory(./lib)

include_directories(
    lib/queue
    lib/system 
    lib/exceptions
)


add_executable(parent parent/parent.cpp)
target_link_libraries(parent PRIVATE shared_queue sys)

add_executable(child child/child.cpp)
target_link_libraries(child PRIVATE shared_queue)

