file(GLOB_RECURSE SDK_SRC
    "${PROJECT_SOURCE_DIR}/src/Awesome/SDK/*"
)

#add_definitions(-DSDK_BUILD_AS_DLL)

add_library(SDK STATIC ${SDK_SRC})
set_target_properties(SDK PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS} -DAWESOME_SDK_STATIC")

install(TARGETS SDK DESTINATION ${PROJECT_SOURCE_DIR})

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	target_link_libraries(SDK Core Lua Windowing Renderer Physics Maths gltf2 Sound libfbxsdk-mdDEBUG)
else()
	target_link_libraries(SDK Core Lua Windowing Renderer Physics Maths gltf2 Sound libfbxsdk-md)
endif()

add_custom_command(OUTPUT "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/libfbxsdk.dll"
	COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/extlibs/sharedlib/${CMAKE_BUILD_TYPE}/libfbxsdk.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/libfbxsdk.dll"
)

add_custom_target(copySDKExtlibs DEPENDS "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/libfbxsdk.dll")
add_dependencies(SDK copySDKExtlibs)

get_directory_property(CUSTOM_DEFINITIONS COMPILE_DEFINITIONS)

#TODO Get Rid of this ?
if(CUSTOM_DEFINITIONS MATCHES ".*WITH_EDITOR")
	target_include_directories(SDK PRIVATE "${CMAKE_SOURCE_DIR}/AwesomeEditor/include")
	target_include_directories(SDK PRIVATE "${CMAKE_SOURCE_DIR}/AwesomeEditor/extlibs/include")
endif()