file(GLOB_RECURSE WINDOWING_SRC
    "${PROJECT_SOURCE_DIR}/src/Awesome/Windowing/*"
)

add_library(Windowing SHARED ${WINDOWING_SRC})
set_target_properties(Windowing PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS} -DAWESOME_WINDOWING_DLL_EXPORTS")

install(TARGETS Windowing DESTINATION ${PROJECT_SOURCE_DIR})
target_link_libraries(Windowing Core)

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
	set(WINDOWING_LIBS
		"${CMAKE_SOURCE_DIR}/AwesomeEngine/build/${CMAKE_BUILD_TYPE}/Windowing.dll"
	)
else()
	set(WINDOWING_LIBS
		"${CMAKE_SOURCE_DIR}/AwesomeEngine/build/${CMAKE_BUILD_TYPE}/Windowing.dll"
		"${CMAKE_SOURCE_DIR}/AwesomeEngine/build/${CMAKE_BUILD_TYPE}/Windowing.ilk"
		"${CMAKE_SOURCE_DIR}/AwesomeEngine/build/${CMAKE_BUILD_TYPE}/Windowing.pdb"
	)
endif()

add_custom_command(TARGET Windowing POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${WINDOWING_LIBS} "${CMAKE_SOURCE_DIR}/AwesomeEditor/build/${CMAKE_BUILD_TYPE}")