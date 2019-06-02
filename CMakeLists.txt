cmake_minimum_required (VERSION 3.8.2)
project (PhysxPlayground)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/CMake/Modules")

set(CMAKE_CXX_FLAGS_DEBUG "-DDEBUG=1 /MTd /Od")
set(CMAKE_CXX_FLAGS_RELEASE "-DAK_OPTIMIZED=1 /MT")
set(CMAKE_C_FLAGS_DEBUG "-DDEBUG=1 /MTd /Od")
set(CMAKE_C_FLAGS_RELEASE "/MT")

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

add_subdirectory(External/bgfx.cmake EXCLUDE_FROM_ALL)
add_subdirectory(External/glfw EXCLUDE_FROM_ALL)

set(USE_MSVC_RUNTIME_LIBRARY_DLL ON)
set(BUILD_SHARED_LIBS OFF)
set(BUILD_UNIT_TESTS OFF CACHE BOOL "Do not build unit tests for bullet.")
set(BUILD_BULLET2_DEMOS OFF CACHE BOOL "no bullet2 demos.")
set(BUILD_CPU_DEMOS OFF CACHE BOOL "no bullet demos.")
set(BUILD_BULLET3 OFF CACHE BOOL "no bullet 3.")

add_subdirectory(External/bullet3 EXCLUDE_FROM_ALL)

add_custom_target(
	build-shaders
	COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/shaders
	COMMAND $<TARGET_FILE:shaderc> --platform windows -p vs_5_0 -f ${CMAKE_CURRENT_SOURCE_DIR}/shaders/vs_cubes.sc 
	-o ${CMAKE_CURRENT_BINARY_DIR}/shaders/vs_cubes.bin --type v -i ${BGFX_DIR}/src --varyingdef ${CMAKE_CURRENT_SOURCE_DIR}/shaders/varying.def.sc
	COMMAND $<TARGET_FILE:shaderc> --platform windows -p ps_5_0 -f ${CMAKE_CURRENT_SOURCE_DIR}/shaders/fs_cubes.sc 
	-o ${CMAKE_CURRENT_BINARY_DIR}/shaders/fs_cubes.bin --type f -i ${BGFX_DIR}/src --varyingdef ${CMAKE_CURRENT_SOURCE_DIR}/shaders/varying.def.sc
)
add_dependencies(build-shaders shaderc)

add_executable(Playground  src/main.cpp 
							src/graphics.cpp 
							src/graphics.h 
							src/simulationWorld.h
							src/simulationWorld.cpp)

add_dependencies(Playground build-shaders)
set_target_properties(Playground PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BUILD_DIR}")
target_include_directories(Playground PRIVATE External/bullet3/src)
target_link_libraries(Playground bgfx bx bimg glfw BulletDynamics BulletCollision LinearMath Bullet3Common)