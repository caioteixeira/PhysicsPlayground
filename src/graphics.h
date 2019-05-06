#pragma once
#include <bgfx/bgfx.h>


struct GLFWwindow;

namespace graphics
{
    static const bgfx::ViewId kClearView = 0;
    int init(GLFWwindow* window);
    void renderFrame();
    void shutdown();

    bgfx::ShaderHandle loadShader(const char *FILENAME);
}