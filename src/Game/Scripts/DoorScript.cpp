#include "DoorScript.h"

#include "../Entities/Plank.h"
#include "../Entities/FixedPoint.h"

#include "../../Engine/Framework/Components/TransformComponent.h"
#include "../../Engine/Framework/Components/MeshRendererComponent.h"
#include "../../Engine/Framework/Components/RigidbodyComponent.h"

#include "PlankScript.h"
#include "FixedPointScript.h"

void DoorScript::Start()
{
	Component::Start();

	transformComponent = GetComponent<TransformComponent>();
	gPhys = GetScene()->physScene->gPhysics;
}

void DoorScript::CreateDoorDefault(PxTransform transform)
{
	transformComponent->SetTransform(transform);
	PxTransform DoorWorldTransform = transformComponent->GetWorldTransform();

	//木板
	auto plank = AddSubGameObject<Plank>("Plank");
	PxQuat rotation = PxQuat(PxPi / 4);
	plank->GetComponent<PlankScript>()->AddPlank(PxTransform(PxVec3(-0.11f,2.0f,0.0f),PxQuat(PxPi/4,PxVec3(0,0,-1))), mPlankGeometry,false);
	auto plankBody = static_cast<PxRigidDynamic*>(plank->GetComponent<RigidbodyComponent>()->GetRigidActor());

	//固定点
	auto fixedPoint = AddSubGameObject<FixedPoint>("FixedPoint");
	fixedPoint->GetComponent<FixedPointScript>()->AddFixedPoint(PxTransform(PxVec3(0, 0, 0)));
	auto fixedBody = static_cast<PxRigidDynamic*>(fixedPoint->GetComponent<RigidbodyComponent>()->GetRigidActor());
	fixedBody->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);//固定点无视物理


	PxRevoluteJoint* joint = PxRevoluteJointCreate(*gPhys,
		fixedBody, PxTransform(PxVec3(-0.01f, 0.0f, 0.0f), PxQuat(PxPi / 2, PxVec3(0, 1, 0))),
		plankBody, PxTransform(PxVec3(0.1f, -2.0f, 0.0), PxQuat(PxPi / 2, PxVec3(0, 1, 0))));

	joint->setLimit(angularLimit);
	joint->setRevoluteJointFlag(PxRevoluteJointFlag::eLIMIT_ENABLED, true);
	
}
