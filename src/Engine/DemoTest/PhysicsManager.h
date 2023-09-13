#pragma once
#include "PxPhysicsAPI.h"
#include "PlayerBall2.h"
#include "InputManager.h"
#include "Enums.h"
#include "../Common/PVD.h"
using namespace physx;

//class ContactReportCallback : public PxSimulationEventCallback
//{
//	void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) { PX_UNUSED(constraints); PX_UNUSED(count); }
//	void onWake(PxActor** actors, PxU32 count) { PX_UNUSED(actors); PX_UNUSED(count); }
//	void onSleep(PxActor** actors, PxU32 count) { PX_UNUSED(actors); PX_UNUSED(count); }
//	void onTrigger(PxTriggerPair* pairs, PxU32 count) { PX_UNUSED(pairs); PX_UNUSED(count); }
//	void onAdvance(const PxRigidBody* const*, const PxTransform*, const PxU32) {}
//	void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
//	{
//		//std::vector<PxContactPairPoint> contactPoints;
//
//		//PxTransform spherePose(PxIdentity);
//		//PxU32 nextPairIndex = 0xffffffff;
//
//		//PxContactPairExtraDataIterator iter(pairHeader.extraDataStream, pairHeader.extraDataStreamSize);
//		//bool hasItemSet = iter.nextItemSet();
//		//if (hasItemSet)
//		//	nextPairIndex = iter.contactPairIndex;
//
//		//for (PxU32 i = 0; i < nbPairs; i++)
//		//{
//		//	//
//		//	// Get the pose of the dynamic object at time of impact.
//		//	//
//		//	if (nextPairIndex == i)
//		//	{
//		//		if (pairHeader.actors[0]->is<PxRigidDynamic>())
//		//			spherePose = iter.eventPose->globalPose[0];
//		//		else
//		//			spherePose = iter.eventPose->globalPose[1];
//
//		//		gContactSphereActorPositions.push_back(spherePose.p);
//
//		//		hasItemSet = iter.nextItemSet();
//		//		if (hasItemSet)
//		//			nextPairIndex = iter.contactPairIndex;
//		//	}
//
//		//	//
//		//	// Get the contact points for the pair.
//		//	//
//		//	const PxContactPair& cPair = pairs[i];
//		//	if (cPair.events & (PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_CCD))
//		//	{
//		//		PxU32 contactCount = cPair.contactCount;
//		//		contactPoints.resize(contactCount);
//		//		cPair.extractContacts(&contactPoints[0], contactCount);
//
//		//		for (PxU32 j = 0; j < contactCount; j++)
//		//		{
//		//			gContactPositions.push_back(contactPoints[j].position);
//		//			gContactImpulses.push_back(contactPoints[j].impulse);
//		//		}
//		//	}
//		//}
//
//		//PxActorFlag
//		//for (PxU32 i = 0; i < nbPairs; i++)
//		//{
//		//	const PxContactPair& cp = pairs[i];
//
//		//	if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
//		//	{
//		//		if ((pairHeader.actors[0]->getName() == BirdName && pairHeader.actors[1]->getName() == PigName) || (pairHeader.actors[1]->getName() == BirdName && pairHeader.actors[0]->getName() == PigName))
//		//		{
//		//			PxActor* otherActor = (pairHeader.actors[0]->getName() == BirdName) ? pairHeader.actors[1] : pairHeader.actors[0];
//		//			PxRigidDynamic* ballPig1 = (PxRigidDynamic*)(otherActor->userData);
//
//
//		//			// insert only once
//		//			if (std::find(ballToDispear.begin(), ballToDispear.end(), ballPig1) == ballToDispear.end())
//		//				ballToDispear.push_back(ballPig1);
//
//		//			std::vector<PxRigidDynamic*>::iterator ballIter = std::find(ballPigList.begin(), ballPigList.end(), ballPig1);
//		//			if (ballIter != ballPigList.end())
//		//			{
//		//				//ballPigList.erase(ballIter);
//		//				//gScene->removeActor(*ballPig1);
//		//			}
//
//
//
//		//			break;
//		//		}
//		//	}
//		//}
//	}
//};
//
//ContactReportCallback gContactReportCallback;

class PhysicsManager
{
private:
	static PhysicsManager* instance;

	PhysicsManager() {}
	~PhysicsManager() {}
	PhysicsManager(const PhysicsManager& other) {}
	const PhysicsManager& operator=(const PhysicsManager& other) {}
public:
	//默认的内存管理和错误报告器
	PxDefaultAllocator		gAllocator;
	PxDefaultErrorCallback	gErrorCallback;

	PxFoundation* gFoundation = NULL;
	PxPhysics* gPhysics = NULL;

	PxDefaultCpuDispatcher* gDispatcher = NULL;
	PxScene* gScene = NULL;

	PxMaterial* gMaterial = NULL;

	PxPvd* gPvd = NULL;

	PlayerBall2* playerBall;

	//PxVec3 a;


	static PhysicsManager* getInstance();

	static void deleteInstance();

	PlayerBall2* getPlayerBall() {
		return playerBall;
	}

	void createStack(const PxTransform& t, PxU32 size, PxReal halfExtent);

	void setupFiltering(PxShape* shape, PxU32 filterGroup, PxU32 filterMask)
	{
		PxFilterData filterData;
		filterData.word0 = filterGroup; // word0 = own ID
		filterData.word1 = filterMask;	// word1 = ID mask to filter pairs that trigger a contact callback;
		shape->setSimulationFilterData(filterData);
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

		pairFlags = PxPairFlag::eCONTACT_DEFAULT
			| PxPairFlag::eDETECT_CCD_CONTACT
			| PxPairFlag::eNOTIFY_TOUCH_CCD
			| PxPairFlag::eNOTIFY_TOUCH_FOUND
			| PxPairFlag::eNOTIFY_CONTACT_POINTS
			| PxPairFlag::eCONTACT_EVENT_POSE;
		return PxFilterFlag::eDEFAULT;
	}
	/*
	PxBase->PxActor->PxRigidActor->PxRigidBody->PxRigidDynamic
	*/
	//创造动态刚体
	PxRigidDynamic* createDynamicBall(PxReal radius, const PxTransform& t, const PxVec3& velocity = PxVec3(0));

	//实例化物理
	void initPhysics();


	//（在render中调用）
	void stepPhysics();


	//清空物理（在render中调用）
	void cleanupPhysics();

};