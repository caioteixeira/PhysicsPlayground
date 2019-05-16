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

struct Element
{
    bx::Vec3 position;
    bx::Vec3 scale;
    bx::Quaternion rotation;
    
    Mesh mesh;

    Element() : position(0, 0, 0), scale(1, 1, 1){}
};

class SimulationWorld
{
public:
    btRigidBody* createCubePhysicsObject(const Element& element, float mass);
    SimulationWorld();
    void simulate();
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
};