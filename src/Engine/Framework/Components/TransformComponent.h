#pragma once
#include "../Component.h"

class TransformComponent :public Component
{
    friend class MeshRendererComponent;
private:
    XMMATRIX transform = XMMatrixIdentity();
    XMMATRIX worldTransform = XMMatrixIdentity();
public:
    TransformComponent* parent = nullptr;
    std::unordered_map<std::string, TransformComponent*> sons;
    bool positionChange = false;
    bool rotationChange = false;
    bool scaleChange = false;
    bool scaleChangeX = false;
    bool scaleChangeY = false;
    bool scaleChangeZ = false;

    static PxVec3 QuaternionToEulerAngles(const XMVECTOR quaternion);
    static float RadiansToDegrees(float radians);
    static PxVec3 RadiansToDegrees(PxVec3 radians);
    static float DegreesToRadians(float degrees);
    static PxVec3 DegreesToRadians(PxVec3 degrees);
    static XMMATRIX MatrixOverlay(XMMATRIX father, XMMATRIX son);

    void UpdateSons();
    void UpdateSelf();

    XMMATRIX GetMatrix() const;
    void SetMatrix(XMMATRIX matrix);
    XMMATRIX GetWorldMatrix() const;
    void SetWorldMatrix(XMMATRIX worldMatrix);
    PxTransform GetTransform() const;
    void SetTransform(PxTransform trans);
    PxTransform GetWorldTransform() const;
    void SetWorldTransform(PxTransform worldTrans);

    PxVec3 GetPosition() const;
    void SetPosition(PxVec3 position);
    PxVec3 GetRotation() const;
    void SetRotation(PxVec3 rotation);
    PxVec4 GetQuat() const;
    void SetQuat(PxVec4& Quat);
    PxVec3 GetScale() const;
    void SetScale(PxVec3 scale);

    void GetAll(PxVec3& pos, PxVec3& rot, PxVec3& sca);

    virtual void ShowDetailUI() override;
};
