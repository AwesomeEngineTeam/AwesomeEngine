cmake_minimum_required(VERSION 3.5)


if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_definitions(-D_DEBUG)
 	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Xclang -std=c++17 -Xclang -fcxx-exceptions -Xclang -O0")
else()
 	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Xclang -std=c++17 -Xclang -fcxx-exceptions -Xclang -O2")
endif()

#TODO Find a less permanent way
add_definitions(-DWITH_EDITOR)

message(STATUS "Using ${CMAKE_CXX_COMPILER_ID} in ${CMAKE_BUILD_TYPE} mode")
message(STATUS "Flags: ${CMAKE_CXX_FLAGS}")

#entrypoints
add_subdirectory(AwesomeEditor)
add_subdirectory(AwesomeEngine)