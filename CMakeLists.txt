cmake_minimum_required (VERSION 3.8.2)
project (PhysxPlayground)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/CMake/Modules")

set(CMAKE_CXX_FLAGS_DEBUG "-DDEBUG=1 /MTd /Od")
set(CMAKE_CXX_FLAGS_RELEASE "-DAK_OPTIMIZED=1 /MT")
set(CMAKE_C_FLAGS_DEBUG "-DDEBUG=1 /MTd /Od")
set(CMAKE_C_FLAGS_RELEASE "/MT")

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

add_subdirectory(External/bgfx.cmake)
add_subdirectory(External/glfw)

add_executable(Playground  src/main.cpp 
							src/graphics.cpp 
							src/graphics.h 
							src/simulationWorld.h
							src/simulationWorld.cpp)
set_target_properties(Playground PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BUILD_DIR}")
target_link_libraries(Playground bgfx bx bimg glfw )