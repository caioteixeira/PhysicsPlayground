#include "graphics.h"

#include <stdio.h>

#include <bx/bx.h>
#include <bx/string.h>
#include <bgfx/platform.h>
#include <GLFW/glfw3.h>
#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#endif
#include <GLFW/glfw3native.h>

#include "simulationWorld.h"

struct PosColorVertex
{
    float x;
    float y;
    float z;
    uint32_t abgr;
};

static PosColorVertex cubeVertices[] =
{
    {-1.0f,  1.0f,  1.0f, 0xff000000 },
    { 1.0f,  1.0f,  1.0f, 0xff0000ff },
    {-1.0f, -1.0f,  1.0f, 0xff00ff00 },
    { 1.0f, -1.0f,  1.0f, 0xff00ffff },
    {-1.0f,  1.0f, -1.0f, 0xffff0000 },
    { 1.0f,  1.0f, -1.0f, 0xffff00ff },
    {-1.0f, -1.0f, -1.0f, 0xffffff00 },
    { 1.0f, -1.0f, -1.0f, 0xffffffff },
};

static const uint16_t cubeTriList[] =
{
    0, 1, 2,
    1, 3, 2,
    4, 6, 5,
    5, 6, 7,
    0, 2, 4,
    4, 2, 6,
    1, 5, 3,
    5, 7, 3,
    0, 4, 1,
    4, 5, 1,
    2, 3, 6,
    6, 3, 7,
};

static void windowResizeCallback(GLFWwindow* window, int width, int height)
{
    bgfx::reset((uint32_t)width, (uint32_t)height, BGFX_RESET_VSYNC);
    bgfx::setViewRect(graphics::kClearView, 0, 0, bgfx::BackbufferRatio::Equal);

    graphics::windowHeight = height;
    graphics::windowWidth = width;
}

int graphics::init(GLFWwindow* window)
{
    glfwSetWindowSizeCallback(window, windowResizeCallback);

    bgfx::renderFrame();
    // Initialize bgfx using the native window handle and window resolution.
    bgfx::Init init;
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
    init.platformData.ndt = glfwGetX11Display();
    init.platformData.nwh = (void*)(uintptr_t)glfwGetX11Window(window);
#elif BX_PLATFORM_OSX
    init.platformData.nwh = glfwGetCocoaWindow(window);
#elif BX_PLATFORM_WINDOWS
    init.platformData.nwh = glfwGetWin32Window(window);
#endif

    glfwGetWindowSize(window, &graphics::windowWidth, &graphics::windowHeight);
    init.resolution.width = (uint32_t)graphics::windowWidth;
    init.resolution.height = (uint32_t)graphics::windowHeight;

    init.resolution.reset = BGFX_RESET_VSYNC;
    if (!bgfx::init(init))
    {
        return 1;
    }

    // Set view 0 to the same dimensions as the window and to clear the color buffer.
    bgfx::setViewClear(kClearView, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0);
    bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
}

void graphics::renderFrame() 
{
    // This dummy draw call is here to make sure that view 0 is cleared if no other draw calls are submitted to view 0.
    bgfx::touch(graphics::kClearView);
    // Use debug font to print information about this example.
    //bgfx::dbgTextClear();
    //bgfx::dbgTextPrintf(0, 0, 0x0f, "Press F1 to toggle stats.");
    //bgfx::dbgTextPrintf(0, 1, 0x0f, "Color can be changed with ANSI \x1b[9;me\x1b[10;ms\x1b[11;mc\x1b[12;ma\x1b[13;mp\x1b[14;me\x1b[0m code too.");
    //bgfx::dbgTextPrintf(80, 1, 0x0f, "\x1b[;0m    \x1b[;1m    \x1b[; 2m    \x1b[; 3m    \x1b[; 4m    \x1b[; 5m    \x1b[; 6m    \x1b[; 7m    \x1b[0m");
    //bgfx::dbgTextPrintf(80, 2, 0x0f, "\x1b[;8m    \x1b[;9m    \x1b[;10m    \x1b[;11m    \x1b[;12m    \x1b[;13m    \x1b[;14m    \x1b[;15m    \x1b[0m");
    //const bgfx::Stats* stats = bgfx::getStats();
    //bgfx::dbgTextPrintf(0, 2, 0x0f, "Backbuffer %dW x %dH in pixels, debug text %dW x %dH in characters.", stats->width, stats->height, stats->textWidth, stats->textHeight);
    // Advance to next frame. Process submitted rendering primitives.
    bgfx::frame();
}

void graphics::shutdown()
{
    bgfx::shutdown();
}

bgfx::ShaderHandle graphics::loadShader(const char *FILENAME)
{
    char filePath[512];

    const char* shaderPath = "???";

    switch (bgfx::getRendererType())
    {
    case bgfx::RendererType::Noop:
    case bgfx::RendererType::Direct3D9:  shaderPath = "shaders/dx9/";   break;
    case bgfx::RendererType::Direct3D11:
    case bgfx::RendererType::Direct3D12: shaderPath = "shaders/dx11/";  break;
    case bgfx::RendererType::Gnm:        shaderPath = "shaders/pssl/";  break;
    case bgfx::RendererType::Metal:      shaderPath = "shaders/metal/"; break;
    case bgfx::RendererType::Nvn:        shaderPath = "shaders/nvn/";   break;
    case bgfx::RendererType::OpenGL:     shaderPath = "shaders/glsl/";  break;
    case bgfx::RendererType::OpenGLES:   shaderPath = "shaders/essl/";  break;
    case bgfx::RendererType::Vulkan:     shaderPath = "shaders/spirv/"; break;

    case bgfx::RendererType::Count:
        BX_CHECK(false, "You should not be here!");
        break;
    }

    bx::strCopy(filePath, BX_COUNTOF(filePath), shaderPath);
    bx::strCat(filePath, BX_COUNTOF(filePath), FILENAME);
    bx::strCat(filePath, BX_COUNTOF(filePath), ".bin");

    FILE *file = fopen(filePath, "rb");
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    const bgfx::Memory *mem = bgfx::alloc(fileSize + 1);
    fread(mem->data, 1, fileSize, file);
    mem->data[mem->size - 1] = '\0';
    fclose(file);

    return bgfx::createShader(mem);
}

void graphics::renderElements(std::vector<Element>& elements)
{
    const bx::Vec3 at = { 0.0f, 0.0f,  0.0f };
    const bx::Vec3 eye = { 20.0f, 20.0f, 20.0f };
    float view[16];
    bx::mtxLookAt(view, eye, at, bx::Vec3(0.f, 1.f, 0.f), bx::Handness::Right);
    float proj[16];

    bx::mtxProj(proj, 60.0f, float(graphics::windowWidth) / float(graphics::windowHeight), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth, bx::Handness::Right);
    bgfx::setViewTransform(0, view, proj);

    for (auto& element : elements)
    {
        float transform[16];
        bx::mtxSRT(transform, element.scale.x, element.scale.y, element.scale.z,
            element.rotation.x, element.rotation.y, element.rotation.z,
            element.position.x, element.position.y, element.position.z);

        bgfx::setTransform(transform);

        bgfx::setVertexBuffer(0, element.mesh.vertexBuffer);
        bgfx::setIndexBuffer(element.mesh.indexBuffer);

        bgfx::submit(0, element.mesh.program);
    }
}

Mesh graphics::createCubeMesh()
{

    bgfx::VertexDecl pcvDecl;
    pcvDecl.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
        .end();

    Mesh mesh;
    mesh.vertexBuffer = bgfx::createVertexBuffer(bgfx::makeRef(cubeVertices, sizeof(cubeVertices)), pcvDecl);
    mesh.indexBuffer = bgfx::createIndexBuffer(bgfx::makeRef(cubeTriList, sizeof(cubeTriList)));

    bgfx::ShaderHandle vsh = graphics::loadShader("vs_cubes");
    bgfx::ShaderHandle fsh = graphics::loadShader("fs_cubes");
    mesh.program = bgfx::createProgram(vsh, fsh, true);

    return mesh;
}
