add_library(Sound INTERFACE)

target_include_directories(Sound INTERFACE ${PROJECT_SOURCE_DIR}/extlibs/include/AkSoundEngine)

if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
	target_link_libraries(Sound INTERFACE AkSoundEngineDLL)
else()
	target_link_libraries(Sound INTERFACE AkSoundEngineDLL-d)
endif()

target_link_libraries(Sound INTERFACE Core)

add_custom_command(OUTPUT "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/AkSoundEngineDLL.dll"
	COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/extlibs/sharedlib/${CMAKE_BUILD_TYPE}/AkSoundEngineDLL.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/AkSoundEngineDLL.dll"
)

add_custom_target(copySoundExtlib DEPENDS "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/AkSoundEngineDLL.dll")
add_dependencies(Sound copySoundExtlib)