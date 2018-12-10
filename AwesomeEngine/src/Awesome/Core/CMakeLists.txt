file(GLOB_RECURSE CORE_SRC
    "${PROJECT_SOURCE_DIR}/src/Awesome/Core/*"
)

add_library(Core SHARED ${CORE_SRC})
set_target_properties(Core PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS} -DAWESOME_CORE_DLL_EXPORTS")

install(TARGETS Core DESTINATION ${PROJECT_SOURCE_DIR})

if(NOT ${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC AND MINGW)
	target_link_libraries(Core stdc++fs m imagehlp)
endif()

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
	set(CORE_LIBS
		"${CMAKE_SOURCE_DIR}/AwesomeEngine/build/${CMAKE_BUILD_TYPE}/Core.dll"
	)
else()
	set(CORE_LIBS
		"${CMAKE_SOURCE_DIR}/AwesomeEngine/build/${CMAKE_BUILD_TYPE}/Core.dll"
		"${CMAKE_SOURCE_DIR}/AwesomeEngine/build/${CMAKE_BUILD_TYPE}/Core.ilk"
		"${CMAKE_SOURCE_DIR}/AwesomeEngine/build/${CMAKE_BUILD_TYPE}/Core.pdb"
	)
endif()

add_custom_command(TARGET Core POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CORE_LIBS} "${CMAKE_SOURCE_DIR}/AwesomeEditor/build/${CMAKE_BUILD_TYPE}")