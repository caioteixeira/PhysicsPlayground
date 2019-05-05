#pragma once
#include <bgfx/bgfx.h>


struct GLFWwindow;

namespace graphics
{
    static const bgfx::ViewId kClearView = 0;
    int init(GLFWwindow* window);
    void renderFrame();
}