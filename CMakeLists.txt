cmake_minimum_required(VERSION 3.0.0)
project(api VERSION 0.1.0)

include(CTest)
enable_testing()

set(CURL_LIBRARY "-lcurl") 
find_package(CURL REQUIRED) 

file(GLOB SOURCES
    "src/*.h"
    "src/*.cpp"
)

add_executable(api ${SOURCES})

include_directories(${CURL_INCLUDE_DIR})
target_link_libraries(api ${CURL_LIBRARIES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
