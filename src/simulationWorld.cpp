#include "simulationWorld.h"

SimulationWorld::SimulationWorld() 
{
    const auto mesh = graphics::createCubeMesh();

    Element cube1;
    cube1.mesh = mesh;

    Element cube2;
    cube2.mesh = mesh;
    cube2.position = bx::Vec3(0, -3, 0);
    cube2.scale = bx::Vec3(5, 0.3f, 5);

    mElements.push_back(cube1);
    mElements.push_back(cube2);

    initPhysics();
}

void SimulationWorld::simulate()
{
    gScene->simulate(1.0f / 60.0f);
    gScene->fetchResults(true);
}

void SimulationWorld::render()
{
    graphics::renderElements(mElements);
}

void SimulationWorld::initPhysics()
{
    
}
