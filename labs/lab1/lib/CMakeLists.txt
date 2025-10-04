set(SYSTEM_HEADERS "system/headers")

set(SYSTEM_SOURCE "system/src")

include_directories(${SYSTEM_HEADERS})
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_library(sys ${SYSTEM_SOURCE}/os_unix.cpp)
else()
    add_library(sys ${SYSTEM_SOURCE}/os_unix.cpp) # dummy cause I only have for linux definition of funcitons/structs
endif()


