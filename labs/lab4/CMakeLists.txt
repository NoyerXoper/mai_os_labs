include_directories()
add_subdirectory(libs)

include_directories(libs/exp_cal libs/translation sys exceptions)

add_executable(statically_linked statically_linked.cpp)

target_link_libraries(statically_linked PUBLIC defexp binarytrans)

add_executable(dynamic_lib_use dynamic_lib_use.cpp)
