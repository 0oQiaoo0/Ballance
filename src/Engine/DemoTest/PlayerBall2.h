#pragma once
#include <mutex>
#include "PxPhysicsAPI.h"

using namespace physx;

class PlayerBall2
{
private:

	PxRigidDynamic* rigidDynamic;

public:
	//PlayerBall() {}

	PlayerBall2(PxPhysics* gPhysics, PxReal radius, const PxTransform& t);

	PxRigidDynamic* getRigidDynamic();

	PxVec3 getPosition();
};