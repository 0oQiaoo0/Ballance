#pragma once
#include <memory>

#include "ContactReportCallback.h"
#include "PxPhysicsAPI.h"
#include "../Particle/ParticleScene.h"

using namespace physx;

class PhysScene
{
private:
    bool startSimulate = true;
    PxDefaultAllocator		gAllocator;
    PxDefaultErrorCallback	gErrorCallback;
public:
    ~PhysScene(void);

    PxRigidActor* CreateTriangleActor(const PxTransform& pose, PxU32 nbVerts, const void* verts, PxU32 triCount, const void* indices32bool, bool IsStatic = true);

    void Initialize();

    void Tick(float DeltaTime);

    //std::vector<std::shared_ptr<PxRigidActor>> rigidActor;

public:
    ContactReportCallback contactReportCallBack;

    PxFoundation* gFoundation = nullptr;
    PxPhysics* gPhysics = nullptr;

    PxDefaultCpuDispatcher* gDispatcher = nullptr;
    PxScene* gScene = nullptr;
    PxCooking* cooking = nullptr;
    PxMaterial* gMaterial = nullptr;

    PxPvd* gPvd = nullptr;

    std::shared_ptr<ParticleScene> particleScene;
};
