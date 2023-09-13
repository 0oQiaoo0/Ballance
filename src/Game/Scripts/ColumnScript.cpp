#include "ColumnScript.h"

#include "../../Engine/Framework/Components/TransformComponent.h"
#include "../../Engine/Framework/Components/RigidbodyComponent.h"
#include "../../Engine/Framework/Components/MeshRendererComponent.h"

void ColumnScript::Start()
{
	transformComponent = GetComponent<TransformComponent>();
	rigidbodyComponent = GetComponent<RigidbodyComponent>();
	meshRendererComponent = GetComponent<MeshRendererComponent>();

	gPhys = GetScene()->physScene->gPhysics;
	cooking = GetScene()->physScene->cooking;
}

void ColumnScript::Update(float DeltaTime)
{
	Component::Update(DeltaTime);
}

void ColumnScript::AddDeafultColumn(float radius, float height, PxTransform transform)
{
	transformComponent->SetTransform(transform);
	auto tmp = transformComponent->GetWorldTransform();//tmp

	// ���涥��
	vertices.push_back(PxVec3(0.0f, height * 0.5f, 0.0f));
	// ���涥��
	vertices.push_back(PxVec3(0.0f, -height * 0.5f, 0.0f));

	// ���涥��
	for (int i = 0; i <= numSegments; i++)
	{
		float angle = angleIncrement * i;
		float x = radius * cosf(angle);
		float z = radius * sinf(angle);

		vertices.push_back(PxVec3(x, height * 0.5f, z));
		vertices.push_back(PxVec3(x, -height * 0.5f, z));
	}

	PxVec3* verticesArray = new PxVec3[vertices.size()];
	std::copy(vertices.begin(), vertices.end(), verticesArray);

	// ����������
	PxConvexMeshDesc convexDesc;
	convexDesc.points.count = vertices.size();
	convexDesc.points.stride = sizeof(PxVec3);
	convexDesc.points.data = verticesArray;
	convexDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX;

	PxDefaultMemoryOutputStream buf;
	PxConvexMeshCookingResult::Enum result;
	if (!cooking->cookConvexMesh(convexDesc, buf, &result));
	else 
	{
		PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
		PxConvexMesh* convexMesh = gPhys->createConvexMesh(input);

		rigidbodyComponent->SetDynamic(PxConvexMeshGeometry(convexMesh), transformComponent->GetWorldTransform(),
			columnStaticFriction, columnDynamicFriction, columnRestitution,
			columnMass, columnLinearDamping, columnAngularDamping,
			columnLinearVelocity, columnAngularVelocity);

		rigidbodyComponent->SetSolverIterationCounts(16, 16);
		meshRendererComponent->RenderDefaultShape(DefaultShape::Cylinder);
	}

	// �ͷŶ��㻺�������ڴ�
	delete[] verticesArray;
}
