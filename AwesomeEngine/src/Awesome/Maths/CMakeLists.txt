file(GLOB MATHS_SRC
    "${PROJECT_SOURCE_DIR}/src/Awesome/Maths/*.cpp"
)

add_library(Maths STATIC ${MATHS_SRC})
set_target_properties(Maths PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS} -DAWESOME_MATHS_DLL_EXPORTS")

install(TARGETS Maths DESTINATION ${PROJECT_SOURCE_DIR})

if(NOT ${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC AND MINGW)
	target_link_libraries(Maths Core m)
else()
	target_link_libraries(Maths Core)
endif()