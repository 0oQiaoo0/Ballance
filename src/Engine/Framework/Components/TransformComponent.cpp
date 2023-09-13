#include "TransformComponent.h"
// 转换函数
PxVec3 TransformComponent::QuaternionToEulerAngles(const XMVECTOR quaternion)
{
    // 将XMVECTOR四元数转换为XMFLOAT4类型
    XMFLOAT4 quat;
    XMStoreFloat4(&quat, quaternion);

    // 计算欧拉角
    PxVec3 euler;
    euler.x = atan2(2.0f * (quat.y * quat.z + quat.w * quat.x), quat.w * quat.w - quat.x * quat.x - quat.y * quat.y + quat.z * quat.z);
    euler.y = asin(-2.0f * (quat.x * quat.z - quat.w * quat.y));
    euler.z = atan2(2.0f * (quat.x * quat.y + quat.w * quat.z), quat.w * quat.w + quat.x * quat.x - quat.y * quat.y - quat.z * quat.z);

    return euler;
}
// 将弧度转换为角度
float TransformComponent::RadiansToDegrees(float radians)
{
    return radians * (180.0f / PxPi);
}
PxVec3 TransformComponent::RadiansToDegrees(PxVec3 radians)
{
    radians.x = RadiansToDegrees(radians.x);
    radians.y = RadiansToDegrees(radians.y);
    radians.z = RadiansToDegrees(radians.z);
    return radians;
}

float TransformComponent::DegreesToRadians(float degrees)
{
    return degrees * (PxPi / 180.0f);
}
PxVec3 TransformComponent::DegreesToRadians(PxVec3 degrees)
{
    degrees.x = DegreesToRadians(degrees.x);
    degrees.y = DegreesToRadians(degrees.y);
    degrees.z = DegreesToRadians(degrees.z);
    return degrees;
}

XMMATRIX TransformComponent::MatrixOverlay(XMMATRIX father, XMMATRIX son)
{
    XMVECTOR _scaleFather, _quaternionFather, _translationFather;
    XMMatrixDecompose(&_scaleFather, &_quaternionFather, &_translationFather, father);
    XMVECTOR _scaleSon, _quaternionSon, _translationSon;
    XMMatrixDecompose(&_scaleSon, &_quaternionSon, &_translationSon, son);
    XMVECTOR worldPosition = XMVectorAdd(_translationFather, XMVector3Rotate(_translationSon, _quaternionFather));
    XMVECTOR worldRotation = XMQuaternionMultiply(_quaternionFather, _quaternionSon);
    XMVECTOR worldScale = XMVectorMultiply(_scaleFather, _scaleSon);
    XMMATRIX worldMatrix = XMMatrixAffineTransformation(worldScale, XMVectorZero(), worldRotation, worldPosition);
    return worldMatrix;
}

void TransformComponent::UpdateSons()
{
    for (auto i : sons)
    {
        i.second->SetMatrix(i.second->GetMatrix());
        i.second->UpdateSons();
    }
}

void TransformComponent::UpdateSelf()
{
    SetMatrix(GetMatrix());
}

XMMATRIX TransformComponent::GetMatrix() const
{
    return transform;
}
void TransformComponent::SetMatrix(XMMATRIX matrix)
{
    transform = matrix;

    if (parent) worldTransform = MatrixOverlay(parent->worldTransform , transform);
    else worldTransform = transform;

    UpdateSons();
}

XMMATRIX TransformComponent::GetWorldMatrix() const
{
    return worldTransform;
}
void TransformComponent::SetWorldMatrix(XMMATRIX worldMatrix)
{
    worldTransform = worldMatrix;

    if (parent) transform = MatrixOverlay(XMMatrixInverse(nullptr, parent->worldTransform) , worldTransform);
    else transform = worldTransform;

    UpdateSons();
}

PxTransform TransformComponent::GetTransform() const
{
    XMVECTOR _scale, _quaternion, _translation;
    XMFLOAT4 quaternion, translation;
    XMMatrixDecompose(&_scale, &_quaternion, &_translation, transform);
    XMStoreFloat4(&quaternion, _quaternion);
    XMStoreFloat4(&translation, _translation);

    return PxTransform(PxVec3(translation.x, translation.y, translation.z),
                       PxQuat(quaternion.x, quaternion.y, quaternion.z, quaternion.w));
}
void TransformComponent::SetTransform(PxTransform trans)
{
    XMVECTOR _scale, _quaternion, _translation;
    XMMatrixDecompose(&_scale, &_quaternion, &_translation, transform);
    _quaternion = XMVectorSet(trans.q.x, trans.q.y, trans.q.z, trans.q.w);
    _translation = XMVectorSet(trans.p.x, trans.p.y, trans.p.z, 0.0f);

    SetMatrix(XMMatrixTransformation(XMVectorZero(), XMQuaternionIdentity(), _scale, XMVectorZero(), _quaternion, _translation));
}

PxTransform TransformComponent::GetWorldTransform() const
{
    XMVECTOR _scale, _quaternion, _translation;
    XMFLOAT4 quaternion, translation;
    XMMatrixDecompose(&_scale, &_quaternion, &_translation, worldTransform);
    XMStoreFloat4(&quaternion, _quaternion);
    XMStoreFloat4(&translation, _translation);

    return PxTransform(PxVec3(translation.x, translation.y, translation.z),
        PxQuat(quaternion.x, quaternion.y, quaternion.z, quaternion.w));
}

void TransformComponent::SetWorldTransform(PxTransform worldTrans)
{
    XMVECTOR _scale, _quaternion, _translation;
    XMMatrixDecompose(&_scale, &_quaternion, &_translation, worldTransform);
    _quaternion = XMVectorSet(worldTrans.q.x, worldTrans.q.y, worldTrans.q.z, worldTrans.q.w);
    _translation = XMVectorSet(worldTrans.p.x, worldTrans.p.y, worldTrans.p.z, 0.0f);
    SetWorldMatrix(XMMatrixTransformation(XMVectorZero(), XMQuaternionIdentity(), _scale, XMVectorZero(), _quaternion, _translation));
}

PxVec3 TransformComponent::GetPosition() const
{
    XMVECTOR _scale, _quaternion, _translation;
    XMFLOAT3 translaton;
    XMMatrixDecompose(&_scale, &_quaternion, &_translation, transform);
    XMStoreFloat3(&translaton, _translation);
    return PxVec3(translaton.x, translaton.y, translaton.z);
}
void TransformComponent::SetPosition(PxVec3 position)
{
    XMVECTOR _scale, _quaternion, _translation;
    XMMatrixDecompose(&_scale, &_quaternion, &_translation, transform);
    _translation = XMVectorSet(position.x, position.y, position.z, 0.0f);
    SetMatrix(XMMatrixScalingFromVector(_scale)*XMMatrixRotationQuaternion(_quaternion)
        *XMMatrixTranslation(position.x,position.y,position.z));
}

PxVec3 TransformComponent::GetRotation() const
{
    XMVECTOR _scale, _quaternion, _translation;
    XMMatrixDecompose(&_scale, &_quaternion, &_translation, transform);
    return RadiansToDegrees(QuaternionToEulerAngles(_quaternion));
}
void TransformComponent::SetRotation(PxVec3 rotation)
{
    rotation = DegreesToRadians(rotation);
    XMVECTOR _scale, _quaternion, _translation;
    XMMatrixDecompose(&_scale, &_quaternion, &_translation, transform);
    _quaternion = XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
    SetMatrix(XMMatrixTransformation(XMVectorZero(), XMQuaternionIdentity(), _scale, XMVectorZero(), _quaternion, _translation));
}

PxVec4 TransformComponent::GetQuat() const
{
    XMVECTOR _scale, _quaternion, _translation;
    XMFLOAT4 quaternion; 
    XMMatrixDecompose(&_scale, &_quaternion, &_translation, transform);
    XMStoreFloat4(&quaternion, _quaternion);
    return PxVec4(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
}

void TransformComponent::SetQuat(PxVec4& Quat)
{
    XMVECTOR _scale, _quaternion, _translation;
    XMMatrixDecompose(&_scale, &_quaternion, &_translation, transform);
    _quaternion = XMVectorSet(Quat.x, Quat.y, Quat.z, Quat.w);
    SetMatrix(XMMatrixTransformation(XMVectorZero(), XMQuaternionIdentity(), _scale, XMVectorZero(), _quaternion, _translation));
}

PxVec3 TransformComponent::GetScale() const
{
    XMVECTOR _scale, _quaternion, _translation;
    XMFLOAT3 scale;
    XMMatrixDecompose(&_scale, &_quaternion, &_translation, transform);
    XMStoreFloat3(&scale, _scale);
    return PxVec3(scale.x, scale.y, scale.z);
}
void TransformComponent::SetScale(PxVec3 scale)
{
    XMVECTOR _scale, _quaternion, _translation;
    XMMatrixDecompose(&_scale, &_quaternion, &_translation, transform);
    _scale = XMVectorSet(scale.x, scale.y, scale.z, 0.0f);
    SetMatrix(XMMatrixTransformation(XMVectorZero(), XMQuaternionIdentity(), _scale, XMVectorZero(), _quaternion, _translation));
}

void TransformComponent::GetAll(PxVec3& pos, PxVec3& rot, PxVec3& sca)
{
    XMVECTOR _scale, _quaternion, _translation;
    XMFLOAT3 scale, translation;
    XMMatrixDecompose(&_scale, &_quaternion, &_translation, transform);

    XMStoreFloat3(&scale, _scale);

    XMStoreFloat3(&translation, _translation);

    sca = PxVec3(scale.x, scale.y, scale.z);
    rot = RadiansToDegrees(QuaternionToEulerAngles(_quaternion));
    pos = PxVec3(translation.x, translation.y, translation.z);
}

void TransformComponent::ShowDetailUI() {
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.9f, 0.2f, 1.0f));
    ImGui::SeparatorText("Transform Component");
    ImGui::PopStyleColor(); // 恢复颜色

    // ??娴?杈??ユ????兼????淇???
    positionChange = false;
    rotationChange = false;
    scaleChange = false;
    scaleChangeX = false;
    scaleChangeY = false;
    scaleChangeZ = false;

    PxVec3 pos, rot, sca;
    GetAll(pos, rot, sca);

    ImGui::Separator();
    // Position
    ImGui::Columns(4);
    ImGui::Text("Position:"); ImGui::NextColumn();
    positionChange |= ImGui::InputFloat("x##TransoformDetailposx", &pos[0], 0.0f); ImGui::NextColumn();
    positionChange |= ImGui::InputFloat("y##TransoformDetailposy", &pos[1], 0.0f); ImGui::NextColumn();
    positionChange |= ImGui::InputFloat("z##TransoformDetailposz", &pos[2], 0.0f);
    ImGui::Columns(1);
    ImGui::Separator();

    // Rotation
    ImGui::Columns(4);
    ImGui::Text("Rotation:"); ImGui::NextColumn();
    rotationChange |= ImGui::InputFloat("x##TransoformDetailrotx", &rot[0], 0.0f); ImGui::NextColumn();
    rotationChange |= ImGui::InputFloat("y##TransoformDetailroty", &rot[1], 0.0f); ImGui::NextColumn();
    rotationChange |= ImGui::InputFloat("z##TransoformDetailrotz", &rot[2], 0.0f);
    ImGui::Columns(1);
    ImGui::Separator();

    // Scale
    ImGui::Columns(4);
    ImGui::Text("Scale:"); ImGui::NextColumn();
    scaleChangeX |= ImGui::InputFloat("x##TransoformDetailScax", &sca[0], 0.0f); ImGui::NextColumn();
    scaleChangeY |= ImGui::InputFloat("y##TransoformDetailScay", &sca[1], 0.0f); ImGui::NextColumn();
    scaleChangeZ |= ImGui::InputFloat("z##TransoformDetailScaz", &sca[2], 0.0f);
    ImGui::Columns(1);
    ImGui::Separator();

    if (positionChange) {
        this->SetPosition(pos);
    }

    if (rotationChange) {
        this->SetRotation(rot);
    }
    scaleChange = scaleChangeX || scaleChangeY || scaleChangeZ;
    if (sca[0] <= 0 || sca[1] <= 0 || sca[2] <= 0) scaleChange = false;
    if (scaleChange) {
        this->SetScale(sca);
    }
}