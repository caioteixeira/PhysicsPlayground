#pragma once

#include<vector>
#include<bx/math.h>
#include <bgfx/bgfx.h>
#include "graphics.h"
#include "BulletCollision/BroadphaseCollision/btBroadphaseInterface.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"
#include "BulletDynamics/ConstraintSolver/btConstraintSolver.h"
#include "BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h"
#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"

struct Color
{
    float r;
    float g;
    float b;
    float a;
};


struct Element
{
    float transform[16];
    Color color;
    
    Mesh mesh;
};

class SimulationWorld
{
public:
    SimulationWorld();

    void createCubeObject(const bx::Vec3 position, const bx::Vec3 scale, const Color color, float mass);
    void simulate(float);
    void render();

private:
    void initPhysics();

    btBroadphaseInterface* m_broadphase;
    btCollisionDispatcher* m_dispatcher;
    btConstraintSolver* m_solver;
    btDefaultCollisionConfiguration* m_collisionConfiguration;
    btDiscreteDynamicsWorld* m_dynamicsWorld;
    std::vector<btCollisionShape*> mCollisionShapes;
    std::vector<btRigidBody*> mRigidBodies;

    std::vector<Element> mElements;

    Mesh mCubeMesh;
};