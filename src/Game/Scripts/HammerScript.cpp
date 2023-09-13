#include "HammerScript.h"
#include "PlankScript.h"

#include "../Entities/FixedPoint.h"
#include "../Entities/Plank.h"

#include "../../Engine/Framework/Components/TransformComponent.h"
#include "../../Engine/Framework/Components/MeshRendererComponent.h"
#include "../../Engine/Framework/Components/RigidbodyComponent.h"

#include "FixedPointScript.h"

void HammerScript::Start()
{
	Component::Start();

	transformComponent = GetComponent<TransformComponent>();
	gPhys = GetScene()->physScene->gPhysics;
}

void HammerScript::CreateHammerDefault(PxTransform transform)
{
	transformComponent->SetTransform(transform);
	PxTransform DoorWorldTransform = transformComponent->GetWorldTransform();

	//固定点
	auto fixedPoint = AddSubGameObject<FixedPoint>("FixedPoint");
	fixedPoint->GetComponent<FixedPointScript>()->AddFixedPoint(PxTransform(PxVec3(0, 0.1 + 0.05 + 2.0 * lHalfLength + 2.0 * plankHalfHeight, 0)));
	auto fixedBody = static_cast<PxRigidDynamic*>(fixedPoint->GetComponent<RigidbodyComponent>()->GetRigidActor());
	fixedBody->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);//固定点无视物理

	//锤子的杆
	auto plank1 = AddSubGameObject<Plank>("Plank1");
	plank1->GetComponent<PlankScript>()->AddPlank(PxTransform(PxVec3(0, 0.1 + 2.0 * lHalfLength + plankHalfHeight, 0)), PxBoxGeometry(sHalfLength, plankHalfHeight, sHalfLength),true);
	auto plank1Body = static_cast<PxRigidDynamic*>(plank1->GetComponent<RigidbodyComponent>()->GetRigidActor());

	//锤头
	auto plank2 = AddSubGameObject<Plank>("Plank2");
	plank2->GetComponent<PlankScript>()->AddPlank(PxTransform(PxVec3(0, 0.1+lHalfLength, 0)), PxBoxGeometry(lHalfLength, lHalfLength, plankHalfHeight),true);
	auto plank2Body = static_cast<PxRigidDynamic*>(plank2->GetComponent<RigidbodyComponent>()->GetRigidActor());

	PxRevoluteJoint* revoluteJoint = PxRevoluteJointCreate(*gPhys,
		plank1Body, PxTransform(PxVec3(0, plankHalfHeight, 0)),
		fixedBody, PxTransform(PxVec3(0, -0.02, 0)));

	PxFixedJoint* fixedJoint = PxFixedJointCreate(*gPhys,
		plank1Body, PxTransform(PxVec3(0, -plankHalfHeight, 0)),
		plank2Body, PxTransform(PxVec3(0, lHalfLength, 0)));


	// 开启摇摆节点的驱动，使其不停摆动
	revoluteJoint->setDriveVelocity(driveVelocity);  // 设置驱动速度
	revoluteJoint->setDriveForceLimit(100.0f);  // 设置驱动力限制
	revoluteJoint->setRevoluteJointFlag(PxRevoluteJointFlag::eDRIVE_ENABLED, true);

}

void HammerScript::CreateHammer(PxTransform transform, PxReal scale, PxReal velocity)
{
	driveVelocity = velocity;
	plankHalfHeight *= scale;
	sHalfLength *= scale;
	lHalfLength *= scale;
	CreateHammerDefault(transform);
}
