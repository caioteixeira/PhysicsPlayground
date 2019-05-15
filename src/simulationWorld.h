#pragma once

#include<vector>
#include<bx/math.h>
#include <bgfx/bgfx.h>
#include "graphics.h"

struct Element
{
    bx::Vec3 position;
    bx::Vec3 rotation;
    bx::Vec3 scale;
    
    Mesh mesh;

    Element() : position(0, 0, 0), rotation(0,0,0), scale(1, 1, 1){}
};

class SimulationWorld
{
public:
    SimulationWorld();
    void simulate();
    void render();

private:
    void initPhysics();

    std::vector<Element> mElements;
};