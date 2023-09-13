#include "RobotBall.h"


#include "../../Engine/UI/imgui/imgui.h"
void RobotBall::Start()
{
	GameObject::Start();

	mGen = std::mt19937(mRandomDevice());
	mRadiusDis = std::uniform_real_distribution<float>(0.0f, mActiveRadius);
	mAngleDis = std::uniform_real_distribution<float>(0.0f, 2.0f * PxPi);

	transformComponent = GetComponent<TransformComponent>();
	rigidbodyComponent = AddComponent<RigidbodyComponent>();
	meshRendererComponent = AddComponent<MeshRendererComponent>();

	transformComponent->SetTransform(PxTransform(mCenterPosition));
	meshRendererComponent->RenderDefaultShape(DefaultShape::Sphere);
	rigidbodyComponent->SetDynamic(PxSphereGeometry(PxReal(0.4f)), transformComponent->GetWorldTransform(), staticFriction, dynamicFriction, restitution, mass, linearDamping, angularDamping, linearVelocity, angularVelocity);
	//rigidbodyComponent->AddToScene();

	mPlayerBall = GetScene()->GetGameObject<PlayerBall>();
	if (meshRendererComponent->renderItem.IsValid())
	{
		meshRendererComponent->renderItem.Material = DefaultMaterial::Tile;
		meshRendererComponent->renderItem.Update();
	}
}


void RobotBall::Update(float DeltaTime)
{
	// Todo: 质量设置比玩家小球大些，力

	if (bAggressive && mPlayerBall) {
		TransformComponent* tsfCpn = mPlayerBall->GetComponent<TransformComponent>();
		if (tsfCpn) {
			mTargetPosition = tsfCpn->GetPosition();
		}
	}
	else {
		mElapsedTime += DeltaTime;
		if (mElapsedTime >= mMovingIntervalTime) {
			mElapsedTime = 0;
			SetNextRandomPosition();
		}
	}

	mCurrentPosition = transformComponent->GetPosition();
	float dx = mTargetPosition.x - mCurrentPosition.x;
	float dz = mTargetPosition.z - mCurrentPosition.z;
	static float minChange = 0.5f;	// 目标位置和当前位置有一定距离才移动，减轻摇摆
	if (bAggressive || fabs(dx) > minChange || fabs(dz) > minChange) {
		PxVec3 dir = PxVec3(dx, 0, dz).getNormalized();
		float force = mPeaceForce;
		if (bAggressive) {
			force = mAggressiveForce;
		}
		PxVec3 forceDir = force * dir;
		rigidbodyComponent->AddForce(forceDir);
	}
}

void RobotBall::setAggressive()
{
	bAggressive = true;
}

void RobotBall::SetNextRandomPosition()
{
	// 生成随机半径和随机角度
	float randomRadius = mRadiusDis(mGen);
	float theta = mAngleDis(mGen);
	// 计算随机位置
	float dx = randomRadius * cos(theta);
	float dz = randomRadius * sin(theta);
	mTargetPosition = PxVec3(mCenterPosition.x + dx, mCenterPosition.y, mCenterPosition.z + dz);
}



void RobotBall::ShowDetailUI()
{
	GameObject::ShowDetailUI();

	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.9f, 0.2f, 1.0f));
	ImGui::SeparatorText("Robot");
	ImGui::PopStyleColor();

	ImGui::Checkbox("Aggressive", &bAggressive);
	ImGui::InputFloat("Active Radius", &mActiveRadius);
	ImGui::InputFloat("Moving Interval Time", &mMovingIntervalTime);
	ImGui::InputFloat3("Center Position", &mCenterPosition.x);
	ImGui::InputFloat3("Target Position", &mTargetPosition.x);

}