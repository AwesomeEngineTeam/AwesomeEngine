cmake_minimum_required(VERSION 3.5)

project(Editor)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE})
link_directories("${PROJECT_SOURCE_DIR}/extlibs/lib")
link_directories("${CMAKE_SOURCE_DIR}/AwesomeEngine/extlibs/lib")

#see https://forum.qt.io/topic/7700/solved-trouble-with-q_object-signals-slots/11
#set(CMAKE_AUTOMOC ON)

file(GLOB_RECURSE EDITOR_SRC
    "${PROJECT_SOURCE_DIR}/src/*"
)

add_executable(Editor ${EDITOR_SRC})

include_directories(include)
include_directories(extlibs/include)
include_directories(${CMAKE_SOURCE_DIR}/AwesomeEngine/include)
include_directories(${CMAKE_SOURCE_DIR}/AwesomeEngine/extlibs/include)
include_directories(${CMAKE_SOURCE_DIR}/AwesomeEngine/extlibs/include/lua)
include_directories(${CMAKE_SOURCE_DIR}/AwesomeEngine/extlibs/include/physX)
include_directories(${CMAKE_SOURCE_DIR}/AwesomeEngine/extlibs/src/vulkan/glslang)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	target_link_libraries(Editor Core Sound Windowing Renderer Maths Physics Lua SDK Qt5Cored Qt5Guid Qt5Widgetsd)
else()
	target_link_libraries(Editor Core Sound Windowing Renderer Maths Physics Lua SDK Qt5Core Qt5Gui Qt5Widgets)
endif()

#copy Awesome Editor extlibs
add_custom_command(TARGET Editor POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/extlibs/sharedlib/${CMAKE_BUILD_TYPE}" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

#copy AkSoundEngineDLL
add_custom_command(OUTPUT "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/AkSoundEngineDLL.dll"
	COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/AwesomeEngine/extlibs/sharedlib/${CMAKE_BUILD_TYPE}/AkSoundEngineDLL.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/AkSoundEngineDLL.dll"
)

add_custom_target(copySoundDLL DEPENDS "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/AkSoundEngineDLL.dll")
add_dependencies(Editor copySoundDLL)

#copy PhysX DLLs
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	set(PHYSX_DLL
		"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/nvToolsExt64_1.dll"
		"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/PhysX3CharacterKinematicDEBUG_x64.dll"
		"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/PhysX3CommonDEBUG_x64.dll"
		"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/PhysX3CookingDEBUG_x64.dll"
		"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/PhysX3DEBUG_x64.dll"
		"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/PhysX3GpuDEBUG_x64.dll"
		"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/PhysXDevice64.dll"
		"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/PxFoundationDEBUG_x64.dll"
		"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/PxPvdSDKDEBUG_x64.dll"
	)
else()
	set(PHYSX_DLL
		"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/nvToolsExt64_1.dll"
		"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/PhysX3CharacterKinematic_x64.dll"
		"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/PhysX3Common_x64.dll"
		"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/PhysX3Cooking_x64.dll"
		"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/PhysX3_x64.dll"
		"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/PhysX3Gpu_x64.dll"
		"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/PhysXDevice64.dll"
		"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/PxFoundation_x64.dll"
		"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/PxPvdSDK_x64.dll"
	)
endif()

add_custom_command(OUTPUT ${PHYSX_DLL}
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/AwesomeEngine/extlibs/sharedlib/${CMAKE_BUILD_TYPE}/physx" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

add_custom_target(copyPhysicsDLL DEPENDS ${PHYSX_DLL})
add_dependencies(Editor copyPhysicsDLL)

#copy fbxsdk DLL
add_custom_command(OUTPUT "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/libfbxsdk.dll"
	COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/AwesomeEngine/extlibs/sharedlib/${CMAKE_BUILD_TYPE}/libfbxsdk.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/libfbxsdk.dll"
)

add_custom_target(copyFBXDLL DEPENDS "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/libfbxsdk.dll")
add_dependencies(Editor copyFBXDLL)