cmake_minimum_required (VERSION 3.8.2)
project (PhysxPlayground)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

add_subdirectory(External/bgfx.cmake)
add_subdirectory(External/glfw)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE headers CONFIGURE_DEPENDS "src/*.h")

add_executable(Playground ${sources} ${headers})
set_target_properties(Playground PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BUILD_DIR}")
target_link_libraries(Playground bgfx bx bimg glfw)