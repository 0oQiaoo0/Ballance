#pragma once
#include "../Component.h"

#include <vector>

class TransformComponent;
class RigidbodyComponent : public Component {
private:
	PxRigidActor* rigidActor = nullptr;
	PxShape* shape = nullptr;
	PxGeometryType::Enum geometryType;
	PxMaterial* material = nullptr;

	TransformComponent* transformComponent = nullptr;
	

	PxPhysics* gPhys = nullptr;
	PxMaterial* gMat = nullptr;


protected:

	
	// ����������Actor�ľ��� �����Ƿ�ģ������
	float checkActorFlagInterval = 0.5f;		// ��� actor �Ƿ�Ҫģ�������ʱ����
	float checkElapsedTime = 0;					// ���ڼ�ʱ
	float disableSimulationDistance_2 = 400;	// ��������ģ��ľ���
	void CheckActorFlagByDistance(float);		// ���ݾ��������Ƿ�ģ������
	void PreUpdate(float DeltaTime) override;

	void PostUpdate(float DeltaTime) override;
public:
	void AddToScene();
	bool isStatic = true;
	void Start() override;
	PxRigidActor* GetRigidActor();

	void SetDynamicDefault(PxGeometry &_geometry,const PxTransform & _worldTransform);

	void SetDynamic(PxGeometry& _geometry, const PxTransform& _worldTransform, const PxReal& _staticFriction, const PxReal& _dynamicFriction, const PxReal& _restitution, const PxReal& _mass, const PxReal& _linearDamping, const PxReal& _angularDamping, const PxVec3& _linearVelocity, PxVec3 _angularVelocity);

	void SetSolverIterationCounts(PxU32 posIters, PxU32 velIters);

	void SetRigidbodyFlag(PxRigidBodyFlag::Enum flag, bool value);

	//void LoadModel(std::string modelPath, bool isStatic = true);

	PxRigidActor* SetTriangleActor(const PxTransform& transform, PxU32 nbVerts, const void* verts, PxU32 triCount, const void* indices32, bool IsStatic);

	void SetTrigger(PxGeometry& _geometry, const PxTransform& _worldTransform);


	void RemoveFromScene();

	void AddForce(PxVec3 force);
	void AddTorque(PxVec3 torque);

	void SetStaticFriction(PxReal friction);
	PxReal GetStaticFriction();
	void SetDynamicFriction(PxReal friction);
	PxReal GetDynamicFriction();
	void SetRestitution(PxReal rest);
	PxReal GetRestitution();
	void SetMass(PxReal mass);
	PxReal GetMass();
	void SetLinearDamping(PxReal linearDamping);
	PxReal GetLinearDamping();
	void SetAngularDamping(PxReal angularDamping);
	PxReal GetAngularDamping();
	void SetLinearVelocity(PxVec3 velocity);
	PxVec3 GetLinearVelocity();
	void SetAngularVelocity(PxVec3 velocity);
	PxVec3 GetAngularVelocity();

	void ShowDetailUI() override;

private:
	PxReal staticFriction = -1;
	PxReal dynamicFriction = -1;
	PxReal restitution = -1;

};