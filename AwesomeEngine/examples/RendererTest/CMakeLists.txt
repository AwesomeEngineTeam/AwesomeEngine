project(RendererTest)

add_executable(RendererTest ${PROJECT_SOURCE_DIR}/main.cpp)
target_link_libraries(RendererTest SDK Lua Core Renderer Windowing Maths Physics Sound)