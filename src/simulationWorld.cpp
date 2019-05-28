#include "simulationWorld.h"

#include <btBulletDynamicsCommon.h>
#include "BulletCollision/CollisionDispatch/btBoxBoxDetector.h"
#include <bx/rng.h>

#include <stdio.h>


void SimulationWorld::createCubeObject(const bx::Vec3 position,  const bx::Vec3 scale, const Color color, float mass)
{
    Element cube;
    cube.mesh = mCubeMesh;
    bx::mtxSRT(cube.transform, scale.x, scale.y, scale.z,
        0.0f, 0.0f, 0.0f,
        position.x, position.y, position.z);
    cube.color = color;

    btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(scale.x), btScalar(scale.y), btScalar(scale.z)));

    mCollisionShapes.push_back(groundShape);

    btTransform groundTransform;
    groundTransform.setIdentity();
    groundTransform.setOrigin(btVector3(position.x, position.y, position.z));

    //rigidbody is dynamic if and only if mass is non zero, otherwise static
    bool isDynamic = (mass > 0.0001f);

    btVector3 localInertia(0, 0, 0);
    if (isDynamic)
        groundShape->calculateLocalInertia(btScalar(mass), localInertia);

    //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
    btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
    btRigidBody* body = new btRigidBody(rbInfo);

    //add the body to the dynamics world
    m_dynamicsWorld->addRigidBody(body);

    mRigidBodies.push_back(body);
    mElements.push_back(cube);
}

SimulationWorld::SimulationWorld() 
{
    initPhysics();

    mCubeMesh = graphics::createCubeMesh();

    //Create ground
    const Color color = { 0.5f, 0.5f, 0.5f, 1.0f };
    createCubeObject(bx::Vec3(0.0f, 0.0f, 0.0f), bx::Vec3(50.f, 0.3f, 50.f), color, 0.f);
}

void SimulationWorld::simulate(float deltaTime)
{
    const float interval = 0.2f;
    static float remaining = 0.0f;
    static bx::RngMwc rng;

    remaining += deltaTime;

    m_dynamicsWorld->updateAabbs();

    if (remaining > interval)
    {
        const auto position = bx::Vec3(bx::frndh(&rng) * 10.f, 30.f, bx::frndh(&rng) * 10.f);
        const Color color = {bx::frnd(&rng), bx::frnd(&rng), bx::frnd(&rng), 1.0f};

        createCubeObject(position, bx::Vec3(1.0f), color, 1);

        remaining = 0.0f;
    }

    m_dynamicsWorld->stepSimulation(deltaTime);
    bgfx::dbgTextClear();

    for (size_t i = 1; i < mRigidBodies.size(); i++)
    {
        btTransform trans;

        mRigidBodies[i]->getMotionState()->getWorldTransform(trans);
        trans.getOpenGLMatrix(mElements[i].transform);
    }
}

void SimulationWorld::render()
{
    graphics::renderElements(mElements);
}

void SimulationWorld::initPhysics()
{
    ///collision configuration contains default setup for memory, collision setup
    m_collisionConfiguration = new btDefaultCollisionConfiguration();
    //m_collisionConfiguration->setConvexConvexMultipointIterations();

    ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);

    m_broadphase = new btDbvtBroadphase();

    ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    btSequentialImpulseConstraintSolver* sol = new btSequentialImpulseConstraintSolver;
    m_solver = sol;

    m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);

    m_dynamicsWorld->setGravity(btVector3(0, -10, 0));
}
