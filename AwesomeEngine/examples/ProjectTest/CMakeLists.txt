project(ProjectTest)

add_executable(ProjectTest ${PROJECT_SOURCE_DIR}/main.cpp)
target_link_libraries(ProjectTest Core SDK)
