#include "PhysScene.h"

#include <iostream>

#include "../Common/PVD.h"

#include "../core/ModelLoader.h"

//struct FilterGroup
//{
//    enum Enum
//    {
//        eBIRD = (1 << 0),
//        ePIG = (1 << 1),
//        eMINE_LINK = (1 << 2),
//        eCRAB = (1 << 3),
//        eHEIGHTFIELD = (1 << 4),
//    };
//};
void setupFiltering(PxShape* shape, PxU32 filterGroup, PxU32 filterMask)
{
    PxFilterData filterData;
    filterData.word0 = filterGroup; // word0 = own ID
    filterData.word1 = filterMask;	// word1 = ID mask to filter pairs that trigger a contact callback;
    shape->setSimulationFilterData(filterData);
}

PxRigidActor* PhysScene::CreateTriangleActor(const PxTransform& pose,
    PxU32 nbVerts,const void* verts,
    PxU32 triCount,const void* indices32,bool IsStatic)
{
  
    if(IsStatic)
    {
        PxTriangleMeshDesc meshDesc;
        meshDesc.points.count           = nbVerts;
        meshDesc.points.stride          = sizeof(ModelLoader::Vertex);
        meshDesc.points.data            = verts;

        meshDesc.triangles.count        = triCount;
        meshDesc.triangles.stride       = 3 * sizeof(UINT32);
        meshDesc.triangles.data         = indices32;
        meshDesc.flags = PxMeshFlags(0);
        PxDefaultMemoryOutputStream writeBuffer;
        PxTriangleMeshCookingResult::Enum result;
        bool res = cooking->validateTriangleMesh(meshDesc);
        PX_ASSERT(res);

        bool status = cooking->cookTriangleMesh(meshDesc, writeBuffer, &result);
        if(!status)
            return NULL;
        // mesh should be validated before cooked without the mesh cleaning

        PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
        PxTriangleMesh* TriangleMesh = gPhysics->createTriangleMesh(readBuffer);
        return PxCreateStatic(*gPhysics, pose, PxTriangleMeshGeometry(TriangleMesh), *gMaterial);
    }
    static const PxVec3 convexVerts[] = {PxVec3(0,1,0),PxVec3(1,0,0),PxVec3(-1,0,0),PxVec3(0,0,1),
    PxVec3(0,0,-1)};

    PxConvexMeshDesc convexDesc;
    convexDesc.points.count           = nbVerts;
    convexDesc.points.stride          = sizeof(ModelLoader::Vertex);
    convexDesc.points.data            = verts;
    convexDesc.flags            = PxConvexFlag::eCOMPUTE_CONVEX ;

#ifdef _DEBUG
    // mesh should be validated before cooking without the mesh cleaning
    //bool res = cooking->validateConvexMesh(convexDesc);
    //PX_ASSERT(res);
#endif
    PxDefaultMemoryOutputStream buf;
    PxConvexMeshCookingResult::Enum result;
    if(!cooking->cookConvexMesh(convexDesc, buf, &result))
        return NULL;
    PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
    PxConvexMesh* convexMesh = gPhysics->createConvexMesh(input);
    return PxCreateDynamic(*gPhysics, pose, PxConvexMeshGeometry(convexMesh), *gMaterial,10);
}

PxFilterFlags contactReportFilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0,
                                        PxFilterObjectAttributes attributes1, PxFilterData filterData1,
                                        PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
    PX_UNUSED(attributes0);
    PX_UNUSED(attributes1);
    PX_UNUSED(filterData0);
    PX_UNUSED(filterData1);
    PX_UNUSED(constantBlockSize);
    PX_UNUSED(constantBlock);


    //
    // Enable CCD for the pair, request contact reports for initial and CCD contacts.
    // Additionally, provide information per contact point and provide the actor
    // pose at the time of contact.
    //

    if (attributes0 & PxFilterObjectFlag::eTRIGGER)
    {
        // attribute0 是触发器对象
        pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
        return PxFilterFlag::eDEFAULT;
    }
    else if (attributes1 & PxFilterObjectFlag::eTRIGGER)
    {
        // attribute1 是触发器对象
        pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
        return PxFilterFlag::eDEFAULT;
    }
    else
    {
        pairFlags = PxPairFlag::eCONTACT_DEFAULT
            | PxPairFlag::eDETECT_CCD_CONTACT
            | PxPairFlag::eNOTIFY_TOUCH_CCD
            | PxPairFlag::eNOTIFY_TOUCH_FOUND
            | PxPairFlag::eNOTIFY_CONTACT_POINTS
            | PxPairFlag::eCONTACT_EVENT_POSE;
        return PxFilterFlag::eDEFAULT;
    }
}

void PhysScene::Initialize()
{
    gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);
    //PVD
    gPvd = physx::PxCreatePvd(*gFoundation);
    PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
    gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
    //创建顶级PxPhysics对象
    gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);
    
    cooking =  PxCreateCooking(PX_PHYSICS_VERSION, *gFoundation,PxCookingParams(PxTolerancesScale()));
    //?缩放
    PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
    //重力
    sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
    gDispatcher = PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher = gDispatcher;
    sceneDesc.filterShader = PxDefaultSimulationFilterShader;
    sceneDesc.filterShader = contactReportFilterShader;//?
    sceneDesc.simulationEventCallback = &contactReportCallBack;
    gScene = gPhysics->createScene(sceneDesc);

    PxInitExtensions(*gPhysics, gPvd);
    PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
    if (pvdClient)
    {
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
    }
    //静摩擦，动摩擦，restitution恢复原状(弹性)
    gMaterial = gPhysics->createMaterial(0.4f, 0.4f, 0.3f);

    particleScene = std::make_shared<ParticleScene>(gFoundation,gPhysics,cooking,gScene,gMaterial);
    particleScene->createParticleContent();
}

PhysScene::~PhysScene(void)
{
    //gFoundation->release();
}

void PhysScene::Tick(float DeltaTime)
{
    PxSceneWriteLock scopedLock(*gScene);
    if (particleScene!=nullptr)
    {
        particleScene->onSubstepSetup(DeltaTime,nullptr);
        //std::cout<<particleScene->testPos.y<<std::endl;
    }
    //时间
    if (startSimulate)
    {
        gScene->simulate(0.014);
    }
   
    gScene->fetchResults(true);
}