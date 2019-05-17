/*
 * Copyright 2011-2019 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */
#include <stdio.h>
#include <bx/bx.h>
#include <bx/math.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <GLFW/glfw3.h>
#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#endif
#include <GLFW/glfw3native.h>

#include "graphics.h"
#include "simulationWorld.h"
#include <chrono>

static bool s_showStats = false;

static void glfw_errorCallback(int error, const char *description)
{
    fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

static void glfw_keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_F1 && action == GLFW_RELEASE)
        s_showStats = !s_showStats;
}

int main(int argc, char **argv)
{
    // Create a GLFW window without an OpenGL context.
    glfwSetErrorCallback(glfw_errorCallback);
    if (!glfwInit())
    {
        return 1;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow *window = glfwCreateWindow(1024, 768, "helloworld", nullptr, nullptr);
    if (!window)
    {
        return 1;
    }

    glfwSetKeyCallback(window, glfw_keyCallback);

    if (!graphics::init(window))
    {
        return 1;
    }

    unsigned int counter = 0;

    SimulationWorld world;
    auto lastFrame = std::chrono::high_resolution_clock::now();
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Enable stats or debug text.
        bgfx::setDebug(s_showStats ? BGFX_DEBUG_STATS : BGFX_DEBUG_TEXT);

        auto now = std::chrono::high_resolution_clock::now();
        auto delta = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(now - lastFrame).count() /
            1000.0f;
        lastFrame = now;

        world.simulate(delta);
        world.render();

        graphics::renderFrame();

        counter++;
    }
    graphics::shutdown();
    glfwTerminate();
    return 0;
}