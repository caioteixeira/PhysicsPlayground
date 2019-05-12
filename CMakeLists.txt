cmake_minimum_required (VERSION 3.8.2)
project (PhysxPlayground)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/CMake/Modules")

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

add_subdirectory(External/bgfx.cmake)
add_subdirectory(External/glfw)

find_package(Physx REQUIRED)

include_directories(${PhysX_INCLUDE_DIR})

add_executable(Playground  src/main.cpp 
							src/graphics.cpp 
							src/graphics.h 
							src/simulationWorld.h
							src/simulationWorld.cpp)
set_target_properties(Playground PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BUILD_DIR}")
target_link_libraries(Playground bgfx bx bimg glfw ${PhysX_LIBRARIES})