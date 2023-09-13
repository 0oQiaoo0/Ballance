#include "PhysicsManager.h"

PhysicsManager* PhysicsManager::instance = new (std::nothrow) PhysicsManager();

PhysicsManager* PhysicsManager::getInstance()
{
	return instance;
}

void PhysicsManager::deleteInstance()
{
	if (instance) {
		delete instance;
		instance = nullptr;
	}
}

//创建立方体堆
void PhysicsManager::createStack(const PxTransform& t, PxU32 size, PxReal halfExtent)
{
	/*
	PxShape 碰撞形状的抽象类;形状是共享的，引用计数的对象。
	可以通过调用PxRigidActor类的createShape()方法或PxPhysics类的createShape()方法来创建实例。
	Box,Sphere,Capsule,Plane,ConvexMesh,TriangleMesh,HeightField
	可以设置Material,offset,flags,name
	*/
	//createShape构建形状;(halfExtent x,y,z)
	PxShape* shape = gPhysics->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *gMaterial);

	setupFiltering(shape, FilterGroup::ePhysicsObject, FilterGroup::eGROUND | FilterGroup::ePhysicsObject);

	for (PxU32 i = 0; i < size; i++)
	{
		for (PxU32 j = 0; j < size; j++)
		{
			//指定位置(-10/-7..9,1,0)(-7..,3,0)(-4..,5,0)...
			PxTransform localTm(PxVec3(PxReal(j * 2) - PxReal(size), PxReal(i * 2 + 1), 0) * halfExtent);
			//createRigidDynamic构建刚体
			PxRigidDynamic* body = gPhysics->createRigidDynamic(t.transform(localTm));
			//attachShape绑定形状到刚体上;
			body->attachShape(*shape);
			//更新质量和惯性（数值表示密度）
			PxRigidBodyExt::updateMassAndInertia(*body, 1.0f);

			body->userData = body;

			//把Actor添加到场景中,注释掉这一句之后所有立方体变得不可见且没有碰撞体积
			gScene->addActor(*body);
		}
	}
	//释放
	shape->release();
}

PxRigidDynamic* PhysicsManager::createDynamicBall(PxReal radius, const PxTransform& t, const PxVec3& velocity)
{

	PxShape* shape = gPhysics->createShape(PxSphereGeometry(radius), *gMaterial);
	setupFiltering(shape, FilterGroup::eGROUND, FilterGroup::ePhysicsObject);

	//PxPhysics object��transform of the new object ��shape of the new object ��the density of the new object(>0)
	PxRigidDynamic* dynamic = gPhysics->createRigidDynamic(t);
	dynamic->attachShape(*shape);

	//设置角阻尼系数，还有线性阻尼linearDamping；线性阻尼控制物理形体或约束抵抗平移的量,而角阻尼控制其抵抗旋转的量。如果设置为0，物体会一直旋转/平移
	dynamic->setAngularDamping(10.0f);
	//设置线性速度
	dynamic->setLinearVelocity(velocity);

	PxRigidBodyExt::updateMassAndInertia(*dynamic, 1.0f);

	gScene->addActor(*dynamic);
	return dynamic;

}

void PhysicsManager::initPhysics() {

	//PxFoundation(版本号,内存回调,错误回调)
	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);
	//PVD
	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
	//创建顶级PxPhysics对象
	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);

	//?缩放
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	//重力
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	//sceneDesc.filterShader = contactReportFilterShader;
	//sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);

	PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}

	//ground
	//静摩擦，动摩擦，restitution恢复原状(弹性)
	gMaterial = gPhysics->createMaterial(0.7f, 0.7f, 0.0f);

	PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, PxPlane(0, 1, 0, 0), *gMaterial);
	gScene->addActor(*groundPlane);

	//playerball
	playerBall = new PlayerBall2(gPhysics, 2, PxTransform(PxVec3(10, 10, 0)));

	gScene->addActor(*playerBall->getRigidDynamic());

	for (PxU32 i = 0; i < 1; i++)
		createStack(PxTransform(PxVec3(0, 0, InputManager::getInstance()->stackZ -= 10.0f)), 1, 2.0f);

	//if (不交互)，在render中把交互设成false就有一个默认的球滚过去撞击堆。
	//if (!interactive)
	//	//PxSphereGeometry Transform,geometry,velocity（速度）
	//	createDynamic(10, PxTransform(PxVec3(0, 40, 100)), PxVec3(0, -50, -100));
}
void PhysicsManager::stepPhysics() {
	//时间
	gScene->simulate(1.0f / 60.0f);
	gScene->fetchResults(true);
}

void PhysicsManager::cleanupPhysics()
{
	//release()销毁对象以及包含的所有对象
	gScene->release();
	gDispatcher->release();
	gPhysics->release();
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();

	gFoundation->release();

	printf("HelloWorld done.\n");
}
