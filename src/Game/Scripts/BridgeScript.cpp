#include "BridgeScript.h"

#include "../Entities/Plank.h"

#include "../../Engine/Framework/Components/TransformComponent.h"
#include "../../Engine/Framework/Components/MeshRendererComponent.h"
#include "../../Engine/Framework/Components/RigidbodyComponent.h"

#include "../Scripts/PlankScript.h"
void BridgeScript::Start()
{
	Component::Start();

	transformComponent = GetComponent<TransformComponent>();
    //transformComponent->SetTransform(bridgeTransform);
    gPhys = GetScene()->physScene->gPhysics;
}

void BridgeScript::CreateBridgeDefault(PxTransform transform)
{
    for (int i = 0; i < plankNum; i++)
    {
        DeleteSubGameObject("Plank_" + to_string(i));
    }
    joints.clear();
    transformComponent->SetTransform(transform);
    PxRigidDynamic* lastRigidbody = nullptr;
    PxTransform bridgeWorldTransform = transformComponent->GetWorldTransform();
    PxReal z = -((plankNum - 1) * plankSpacing) / 2;
    for (int i = 0; i < plankNum; i++, z += plankSpacing)
    {
        auto plank = AddSubGameObject<Plank>("Plank_" + to_string(i));

        plank->GetComponent<PlankScript>()->AddPlankDefault(PxTransform(PxVec3(0, 0, z)));

        auto rigidbody = static_cast<PxRigidDynamic*>(plank->GetComponent<RigidbodyComponent>()->GetRigidActor());
        if (i == 0)
        {
            rigidbody->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
        }
        else
        {
            if (i == plankNum - 1)
            {
                rigidbody->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
            }
            PxD6Joint* j1 = PxD6JointCreate(*gPhys,
                lastRigidbody, PxTransform(PxVec3(jointOffset, 0, plankSpacing / 2)), 
                rigidbody, PxTransform(PxVec3(jointOffset, 0, -plankSpacing / 2)));
            
            //允许木板绕x轴旋转
            j1->setMotion(PxD6Axis::eTWIST, PxD6Motion::eLIMITED);
            j1->setTwistLimit(twistLimit);

            //允许木板沿y轴上下移动
            j1->setMotion(PxD6Axis::eY, PxD6Motion::eLIMITED);
            j1->setLinearLimit(linearLimit);
            
            j1->setBreakForce(breakForce, breakTorque);
            j1->setProjectionLinearTolerance(projectionLinearTolerance);
            joints.push_back(j1);

            PxD6Joint* j2 = PxD6JointCreate(*gPhys,
                lastRigidbody, PxTransform(PxVec3(-jointOffset, 0, plankSpacing / 2)),
                rigidbody, PxTransform(PxVec3(-jointOffset, 0, -plankSpacing / 2)));

            //允许木板绕x轴旋转
            j2->setMotion(PxD6Axis::eTWIST, PxD6Motion::eLIMITED);
            j2->setTwistLimit(twistLimit);

            //允许木板沿y轴上下运动
            j2->setMotion(PxD6Axis::eY, PxD6Motion::eLIMITED);
            j2->setLinearLimit(linearLimit);

            j2->setBreakForce(breakForce, breakTorque);
            j2->setProjectionLinearTolerance(projectionLinearTolerance);
            joints.push_back(j2);
        }
        lastRigidbody = rigidbody;
    }
}

void BridgeScript::CreateBridge(PxTransform transform, int plankNum)
{
    this->plankNum = plankNum;
    CreateBridgeDefault(transform);
}
