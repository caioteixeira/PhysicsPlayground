#include "simulationWorld.h"

#include "PxPhysicsAPI.h"

physx::PxDefaultAllocator		gAllocator;
physx::PxDefaultErrorCallback	gErrorCallback;

physx::PxFoundation*			gFoundation = NULL;
physx::PxPhysics*				gPhysics = NULL;

physx::PxDefaultCpuDispatcher*	gDispatcher = NULL;
physx::PxScene*				gScene = NULL;

physx::PxMaterial*				gMaterial = NULL;

physx::PxPvd*                  gPvd = NULL;

SimulationWorld::SimulationWorld() 
{
    const auto mesh = graphics::createCubeMesh();

    Element cube1;
    cube1.mesh = mesh;

    Element cube2;
    cube2.mesh = mesh;
    cube2.position = bx::Vec3(0, -3, 0);
    cube2.scale = bx::Vec3(5, 0.3f, 5);

    physx::PxShape* shape = gPhysics->createShape(physx::PxBoxGeometry(0.5, 0.5, 0.5), *gMaterial);
    
    physx::PxTransform localTm(physx::PxVec3(physx::PxReal(0) - physx::PxReal(0), physx::PxReal(0), 0));
    physx::PxRigidDynamic* body = gPhysics->createRigidDynamic(localTm);
    body->attachShape(*shape);
    physx::PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
    gScene->addActor(*body);

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
    using namespace physx;

     gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
    
     gPvd = PxCreatePvd(*gFoundation);
     PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
     gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
    
     gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);
    
     PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
     sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
     gDispatcher = PxDefaultCpuDispatcherCreate(2);
     sceneDesc.cpuDispatcher = gDispatcher;
     sceneDesc.filterShader = PxDefaultSimulationFilterShader;
     gScene = gPhysics->createScene(sceneDesc);
    
     PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
     if (pvdClient)
     {
         pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
         pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
         pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
     }
     gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);
    
     physx::PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, physx::PxPlane(0, 1, 0, 0), *gMaterial);
     gScene->addActor(*groundPlane);
}
