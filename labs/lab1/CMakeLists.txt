set(SYSTEM_HEADERS "lib/system/headers")
set(SCANNER_HEADERS "lib/scanner/headers")

include_directories(${SYSTEM_HEADERS} ${SCANNER_HEADERS})

add_subdirectory(./lib)


add_executable(parent parent/parent.cpp)
target_link_libraries(parent PRIVATE sys)

add_executable(child child/child.cpp)
target_link_libraries(child PRIVATE sys)

