cmake_minimum_required(VERSION 2.6)
file(GLOB_RECURSE RENDERER_SRC
    "${PROJECT_SOURCE_DIR}/src/Awesome/Renderer/*"
    "${PROJECT_SOURCE_DIR}/extlibs/include/mikktspace.c"
)

add_library(Renderer SHARED ${RENDERER_SRC})
set_target_properties(Renderer PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS} -DAWESOME_RENDERER_DLL_EXPORTS -DUNICODE")

install(TARGETS Renderer DESTINATION ${PROJECT_SOURCE_DIR})

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	target_link_libraries(Renderer Core Maths Windowing ImGUId SPIRV)
else()
	target_link_libraries(Renderer Core Maths Windowing ImGUI SPIRV)
endif()

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
	set(RENDERER_LIBS
		"${CMAKE_SOURCE_DIR}/AwesomeEngine/build/${CMAKE_BUILD_TYPE}/Renderer.dll"
	)
elseif (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	set(RENDERER_LIBS
		"${CMAKE_SOURCE_DIR}/AwesomeEngine/build/${CMAKE_BUILD_TYPE}/Renderer.dll"
		"${CMAKE_SOURCE_DIR}/AwesomeEngine/build/${CMAKE_BUILD_TYPE}/Renderer.ilk"
		"${CMAKE_SOURCE_DIR}/AwesomeEngine/build/${CMAKE_BUILD_TYPE}/Renderer.pdb"
	)
else()
	set(RENDERER_LIBS
		"${CMAKE_SOURCE_DIR}/AwesomeEngine/build/${CMAKE_BUILD_TYPE}/Renderer.dll"
		"${CMAKE_SOURCE_DIR}/AwesomeEngine/build/${CMAKE_BUILD_TYPE}/Renderer.pdb"
	)
endif()

add_custom_command(TARGET Renderer POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${RENDERER_LIBS} "${CMAKE_SOURCE_DIR}/AwesomeEditor/build/${CMAKE_BUILD_TYPE}")