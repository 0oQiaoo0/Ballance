#include "FenceScript.h"

#include "../Entities/Plank.h"

#include "../../Engine/Framework/Components/TransformComponent.h"
#include "../../Engine/Framework/Components/MeshRendererComponent.h"
#include "../../Engine/Framework/Components/RigidbodyComponent.h"

#include "FenceScript.h"
#include "PlankScript.h"

void FenceScript::Start()
{
	Component::Start();

	transformComponent = GetComponent<TransformComponent>();
	gPhys = GetScene()->physScene->gPhysics;
}

void FenceScript::CreateFenceDefault(PxTransform transform)
{
	transformComponent->SetTransform(transform);
	PxTransform FenceWorldTransform = transformComponent->GetWorldTransform();

	//¸Ë
	auto plank = AddSubGameObject<Plank>("Plank");
	plank->GetComponent<PlankScript>()->AddPlank(PxTransform(PxVec3(0.0f, 0.0f, 0.0f)), mPlankGeometry,
		plankStaticFriction, plankDynamicFriction, plankRestitution, plankMass, plankLinearDamping, plankAngularDamping, true);
	auto plankBody = static_cast<PxRigidDynamic*>(plank->GetComponent<RigidbodyComponent>()->GetRigidActor());

	//Õ¤À¸
	auto fence1 = AddSubGameObject<Plank>("Fence1");
	fence1->GetComponent<PlankScript>()->AddPlank(PxTransform(PxVec3(0.0f,0.75f, -0.25f)), mFenceGeometry,
		plankStaticFriction, plankDynamicFriction, plankRestitution, plankMass, plankLinearDamping, plankAngularDamping, true);
	auto fence1Body = static_cast<PxRigidDynamic*>(fence1->GetComponent<RigidbodyComponent>()->GetRigidActor());

	auto fence2 = AddSubGameObject<Plank>("Fence2");
	fence2->GetComponent<PlankScript>()->AddPlank(PxTransform(PxVec3(0.0f, 0.75f, 0.15f)), mFenceGeometry,
		plankStaticFriction, plankDynamicFriction, plankRestitution, plankMass, plankLinearDamping, plankAngularDamping, true);
	auto fence2Body = static_cast<PxRigidDynamic*>(fence2->GetComponent<RigidbodyComponent>()->GetRigidActor());

	//ÍÆ°å
	auto push = AddSubGameObject<Plank>("Push");
	push->GetComponent<PlankScript>()->AddPlank(PxTransform(PxVec3(0.0f, 0.0f, 1.6f)), mPushGeometry,
		plankStaticFriction, plankDynamicFriction, plankRestitution, plankMass, plankLinearDamping, plankAngularDamping, true);
	auto pushBody = static_cast<PxRigidDynamic*>(push->GetComponent<RigidbodyComponent>()->GetRigidActor());

	PxFixedJoint* pfj_1 = PxFixedJointCreate(*gPhys,
		plankBody,PxTransform(PxVec3(0,0,1.5)),
		pushBody,PxTransform(PxVec3(0,-0.25,-0.1)));

	PxFixedJoint* pfj_2 = PxFixedJointCreate(*gPhys,
		plankBody, PxTransform(PxVec3(0, 0.15, -0.25)),
		fence1Body, PxTransform(PxVec3(0, -0.6, 0.0)));

	PxFixedJoint* pfj_3 = PxFixedJointCreate(*gPhys,
		plankBody, PxTransform(PxVec3(0, 0.15, 0.15)),
		fence2Body, PxTransform(PxVec3(0, -0.6, 0.0)));
}
