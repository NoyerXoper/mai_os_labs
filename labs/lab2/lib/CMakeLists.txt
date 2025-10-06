include_directories(factorial matrix permutations "system" determinant)

add_library(matrix matrix/matrix.cpp)

add_library(permutations permutations/permutations.cpp)

add_library(systemLib "system/threads.cpp")

add_library(determinant determinant/determinant.cpp)
target_link_libraries(determinant PRIVATE matrix permutations systemLib)
