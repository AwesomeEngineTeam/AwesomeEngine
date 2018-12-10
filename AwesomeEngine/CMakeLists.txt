cmake_minimum_required(VERSION 2.6)

project(Engine)

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE})
#link_directories("${CMAKE_SOURCE_DIR}/extlibs/lib")
#
#if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
# 	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Xclang -std=c++17 -Xclang -fcxx-exceptions")
#elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Intel")
#	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
#	set(BUILD_ARCH "-m64")
#elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
#	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
#endif()
#
#if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
#    add_definitions(-D_DEBUG)
#endif()
#
#add_definitions(-DWITH_EDITOR)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE})
link_directories("${PROJECT_SOURCE_DIR}/extlibs/lib")

include_directories(include)
include_directories(extlibs/include)
include_directories(extlibs/include/lua)
include_directories(extlibs/include/physX)
include_directories(extlibs/include/vulkan)
include_directories(extlibs/src/vulkan/glslang)

#dlls
add_subdirectory(src/Awesome/Core)
add_subdirectory(src/Awesome/Maths)
add_subdirectory(src/Awesome/Physics)
add_subdirectory(src/Awesome/Renderer)
add_subdirectory(src/Awesome/Windowing)
add_subdirectory(src/Awesome/SDK)

add_subdirectory(extlibs/src/lua)
add_subdirectory(extlibs/src/vulkan/glslang/SPIRV)
add_subdirectory(extlibs/src/vulkan/glslang/glslang)
add_subdirectory(extlibs/src/vulkan/glslang/OGLCompilersDLL)
add_subdirectory(extlibs/src/gltf2)

#interfaces
add_subdirectory(include/Awesome/Sound)

#entrypoints
add_subdirectory(examples/RendererTest)
add_subdirectory(examples/ProjectTest)
add_subdirectory(examples/FileSystem)