cmake_minimum_required (VERSION 3.8.2)
project (PhysxPlayground)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

add_subdirectory(External/bgfx.cmake)
add_subdirectory(External/glfw)

add_executable(Playground Src/main.cpp)
set_target_properties(Playground PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
target_link_libraries(Playground bgfx bx bimg glfw)