#include "math.h"

DirectX::XMFLOAT4 ToEulerAngles(DirectX::XMFLOAT4 q)
{
    DirectX::XMFLOAT4 angles;
 
    // roll (x-axis rotation)
    double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
    double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
    angles.z = std::atan2(sinr_cosp, cosr_cosp);
 
    // pitch (y-axis rotation)
    double sinp = 2 * (q.w * q.y - q.z * q.x);
    if (std::abs(sinp) >= 1)
        angles.x = std::copysign(DirectX::XM_PI / 2, sinp); // use 90 degrees if out of range
    else
        angles.x = std::asin(sinp);
 
    // yaw (z-axis rotation)
    double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
    double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
    angles.y = std::atan2(siny_cosp, cosy_cosp);
    angles.z = 1;
    return angles;
}

DirectX::XMFLOAT4 ToEulerAngles(DirectX::XMVECTOR qv){
    DirectX::XMFLOAT4 q;
    DirectX::XMStoreFloat4(&q,qv);
    return ToEulerAngles(q);
}

physx::PxTransform XMMATRIX_to_PxTransform(DirectX::XMMATRIX& matrix)
{
    physx::PxVec3 scale;
    return XMMATRIX_to_PxTransform(matrix,scale);
}

physx::PxTransform XMMATRIX_to_PxTransform(DirectX::XMMATRIX& matrix, physx::PxVec3& OutScale)
{

    DirectX::XMVECTOR _scale, _quaternion, _translation;
    DirectX::XMFLOAT4 quaternion, translation,scale;
    DirectX::XMMatrixDecompose(&_scale, &_quaternion, &_translation, matrix);
    DirectX::XMStoreFloat4(&quaternion, _quaternion);
    DirectX::XMStoreFloat4(&translation, _translation);
    DirectX::XMStoreFloat4(&scale, _scale);
    physx::PxTransform px_transform(translation.x,translation.y,translation.z,
        physx::PxQuat(quaternion.x,quaternion.y,quaternion.z,quaternion.w));
    OutScale.x = scale.x;
    OutScale.y = scale.y;
    OutScale.z = scale.z;
    return px_transform;
}

physx::PxTransform XMFloat4x4_to_PxTransform(DirectX::XMFLOAT4X4& float4x4)
{
    physx::PxVec3 scale;
    return XMFloat4x4_to_PxTransform(float4x4,scale);
}

physx::PxTransform XMFloat4x4_to_PxTransform(DirectX::XMFLOAT4X4& float4x4, physx::PxVec3& OutScale)
{
    DirectX::XMMATRIX matrix = DirectX::XMLoadFloat4x4(&float4x4);
    return XMMATRIX_to_PxTransform(matrix,OutScale);
}

DirectX::XMMATRIX PxTransform_to_XMMATRIX(physx::PxTransform& px_transform)
{
    physx::PxVec3 scale;
    return PxTransform_to_XMMATRIX(px_transform,scale);
}

DirectX::XMMATRIX PxTransform_to_XMMATRIX(physx::PxTransform& px_transform, physx::PxVec3& Scale)
{
    physx::PxVec3 p = px_transform.p;
    physx::PxQuat q = px_transform.q;
    DirectX::XMMATRIX matrix =
        DirectX::XMMatrixScaling(Scale.x,Scale.y,Scale.z)
    *DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&DirectX::XMFLOAT4(q.x,q.y,q.z,q.w)))
    *DirectX::XMMatrixTranslation(p.x,p.y,p.z);
    return matrix;
}

DirectX::XMFLOAT4X4 PxTransform_to_XMFloat4x4(physx::PxTransform& px_transform)
{
    physx::PxVec3 scale = {1,1,1};
    return PxTransform_to_XMFloat4x4(px_transform,scale);
}

DirectX::XMFLOAT4X4 PxTransform_to_XMFloat4x4(physx::PxTransform& px_transform, physx::PxVec3& Scale)
{
    DirectX::XMMATRIX matrix = PxTransform_to_XMMATRIX(px_transform,Scale);
    DirectX::XMFLOAT4X4 float4x4;
    DirectX::XMStoreFloat4x4(&float4x4,matrix);
    return float4x4;
}
