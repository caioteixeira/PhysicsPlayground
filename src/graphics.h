#pragma once
#include <bgfx/bgfx.h>
#include <vector>
#include "bx/math.h"


struct GLFWwindow;

struct Mesh
{
    bgfx::ProgramHandle program;
    bgfx::VertexBufferHandle vertexBuffer;
    bgfx::IndexBufferHandle indexBuffer;
};

struct Element;

namespace graphics
{
    static const bgfx::ViewId kClearView = 0;
    static int windowWidth;
    static int windowHeight;
    static bgfx::UniformHandle kColorUniform;

    int init(GLFWwindow* window);
    void renderFrame();
    void shutdown();

    bgfx::ShaderHandle loadShader(const char *FILENAME);

    void renderElements(std::vector<Element>& elements);

    Mesh createCubeMesh();
}