#include "simulationWorld.h"

#include <btBulletDynamicsCommon.h>
#include "BulletCollision/CollisionDispatch/btBoxBoxDetector.h"


btRigidBody* SimulationWorld::createCubePhysicsObject(const Element& element, float mass)
{
    btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(element.scale.x), btScalar(element.scale.y), btScalar(element.scale.z)));

    mCollisionShapes.push_back(groundShape);

    btTransform groundTransform;
    groundTransform.setIdentity();
    groundTransform.setOrigin(btVector3(element.position.x, element.position.y, element.position.z));

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

    return body;
}

SimulationWorld::SimulationWorld() 
{
    initPhysics();

    //Create ground
    const auto mesh = graphics::createCubeMesh(0x33333333);

    Element cube2;
    cube2.mesh = mesh;
    cube2.scale = bx::Vec3(50, 0.3f, 50);
    const auto rigidBody2 = createCubePhysicsObject(cube2, 0);
    mRigidBodies.push_back(rigidBody2);
    mElements.push_back(cube2);
}

void SimulationWorld::simulate(float deltaTime)
{
    const float interval = 0.2f;
    static float remaining = 0.0f;

    remaining += deltaTime;

    if (remaining > interval)
    {
        const auto mesh = graphics::createCubeMesh(0x77733333);

        Element cube1;
        cube1.position = bx::Vec3(0, 5, 0);
        cube1.mesh = mesh;

        const auto rigidBody = createCubePhysicsObject(cube1, 1);
        mRigidBodies.push_back(rigidBody);
        mElements.push_back(cube1);

        remaining = 0.0f;
    }

    m_dynamicsWorld->stepSimulation(deltaTime);
    bgfx::dbgTextClear();

    for (int i = 0; i < mRigidBodies.size(); i++)
    {
        btTransform trans;
        mRigidBodies[i]->getMotionState()->getWorldTransform(trans);
        auto origin = trans.getOrigin();
        auto rotation = trans.getRotation();
        mElements[i].position = bx::Vec3(origin.getX(), origin.getY(), origin.getZ());
        mElements[i].rotation.x = rotation.getX();
        mElements[i].rotation.y = rotation.getY();
        mElements[i].rotation.z = rotation.getZ();
        mElements[i].rotation.w = rotation.getW();
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
