#pragma once
#include <cmath>
#include <DirectXMath.h>
#include <foundation/PxTransform.h>

#include "d3d12.h"
DirectX::XMFLOAT4 ToEulerAngles(DirectX::XMFLOAT4 quaternion);
DirectX::XMFLOAT4 ToEulerAngles(DirectX::XMVECTOR quaternion);
physx::PxTransform XMMATRIX_to_PxTransform(DirectX::XMMATRIX& matrix);
physx::PxTransform XMMATRIX_to_PxTransform(DirectX::XMMATRIX& matrix,physx::PxVec3& OutScale);
physx::PxTransform XMFloat4x4_to_PxTransform(DirectX::XMFLOAT4X4& float4x4);
physx::PxTransform XMFloat4x4_to_PxTransform(DirectX::XMFLOAT4X4& float4x4,physx::PxVec3& OutScale);
DirectX::XMMATRIX PxTransform_to_XMMATRIX(physx::PxTransform& px_transform);
DirectX::XMMATRIX PxTransform_to_XMMATRIX(physx::PxTransform& px_transform,physx::PxVec3& Scale);
DirectX::XMFLOAT4X4 PxTransform_to_XMFloat4x4(physx::PxTransform& px_transform);
DirectX::XMFLOAT4X4 PxTransform_to_XMFloat4x4(physx::PxTransform& px_transform,physx::PxVec3& Scale);