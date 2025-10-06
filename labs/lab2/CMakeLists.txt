add_subdirectory(lib)

include_directories(lib/determinant lib/matrix lib/factorial)
add_executable(main main.cpp)
target_link_libraries(main PRIVATE determinant)