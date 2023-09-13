#include "PlayerBall2.h"

PlayerBall2::PlayerBall2(PxPhysics* gPhysics, PxReal radius, const PxTransform& t)
{
	//设置摩擦力
	float staticFriction = 0.7f;
	float dynamicFriction = 0.7f;
	PxMaterial* materialPath = gPhysics->createMaterial(staticFriction, dynamicFriction, 0);

	//Create the ball shape with radius and NULL material.
	PxShape* shape = gPhysics->createShape(PxSphereGeometry(radius), *materialPath);
	//Setup collision filter for the shape.
	//setupFiltering(shape, FilterGroup::eGROUND, FilterGroup::eGROUND | FilterGroup::ePhysicsObject);

	//PxPhysics object��transform of the new object ��shape of the new object ��the density of the new object(>0)
	PxRigidDynamic* dynamic = gPhysics->createRigidDynamic(t);

	dynamic->attachShape(*shape);

	//设置线性阻尼
	dynamic->setLinearDamping(0.4f);
	//设置角阻尼
	dynamic->setAngularDamping(0.1f);
	//设置线性速度
	dynamic->setLinearVelocity(PxVec3(0, 0, 0));
	//设置角速度
	dynamic->setAngularVelocity(PxVec3(0, 0, 0));
	//更新刚体的质量和惯性矩阵
	PxRigidBodyExt::updateMassAndInertia(*dynamic, 1.0f);

	//gScene->addActor(*dynamic);

	rigidDynamic = dynamic;

	//int a  = 1;
	/*return dynamic;*/
}

PxRigidDynamic* PlayerBall2::getRigidDynamic()
{
	return rigidDynamic;
}

PxVec3 PlayerBall2::getPosition()
{
	return rigidDynamic->getGlobalPose().p;
}
