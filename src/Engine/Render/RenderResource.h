#pragma once

#include "dx12/Common/d3dApp.h"
#include "dx12/Common/MathHelper.h"
#include "dx12/Common/UploadBuffer.h"
#include "dx12/Common/GeometryGenerator.h"
#include "dx12/Common/Camera.h"
#include "dx12/FrameResource.h"
#include "dx12/ShadowMap.h"
#include "dx12/Ssao.h"
#include "dx12/SkinnedData.h"
#include "dx12/LoadM3d.h"
using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

struct SubMesh
{
    Material* material;
    UINT IndexCount = 0;
    UINT StartIndexLocation = 0;
    INT BaseVertexLocation = 0;
    
    // Bounding box of the geometry defined by this submesh. 
    // This is used in later chapters of the book.
    DirectX::BoundingBox Bounds;
};

struct SkinnedModelInstance
{
    SkinnedData* SkinnedInfo = nullptr;
    std::vector<DirectX::XMFLOAT4X4> FinalTransforms;
    std::string ClipName;
    float TimePos = 0.0f;

    // Called every frame and increments the time position, interpolates the 
    // animations for each bone based on the current animation clip, and 
    // generates the final transforms which are ultimately set to the effect
    // for processing in the vertex shader.
    void UpdateSkinnedAnimation(float dt)
    {
        TimePos += dt;

        // Loop animation
        if (TimePos > SkinnedInfo->GetClipEndTime(ClipName))
            TimePos = 0.0f;

        // Compute the final transforms for this time position.
        SkinnedInfo->GetFinalTransforms(ClipName, TimePos, FinalTransforms);
    }
};

// Lightweight structure stores parameters to draw a shape.  This will
// vary from app-to-app.
struct RenderItem
{
    RenderItem() = default;
    RenderItem(const RenderItem& rhs) = delete;

    // World matrix of the shape that describes the object's local space
    // relative to the world space, which defines the position, orientation,
    // and scale of the object in the world.
    XMFLOAT4X4 World = MathHelper::Identity4x4();

    XMFLOAT4X4 TexTransform = MathHelper::Identity4x4();

    // Dirty flag indicating the object data has changed and we need to update the constant buffer.
    // Because we have an object cbuffer for each FrameResource, we have to apply the
    // update to each FrameResource.  Thus, when we modify obect data we should set 
    // NumFramesDirty = gNumFrameResources so that each frame resource gets the update.
    int NumFramesDirty = gNumFrameResources2;

    // Index into GPU constant buffer corresponding to the ObjectCB for this render item.
    UINT ObjCBIndex = -1;

    Material* Mat = nullptr;
    MeshGeometry* Geo = nullptr;

    // Primitive topology.
    D3D12_PRIMITIVE_TOPOLOGY PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    // DrawIndexedInstanced parameters.
    UINT IndexCount = 0;
    UINT StartIndexLocation = 0;
    int BaseVertexLocation = 0;

    // Only applicable to skinned render-items.
    UINT SkinnedCBIndex = -1;

    DirectX::BoundingBox boundingBox;
    // nullptr if this render-item is not animated by skinned mesh.
    SkinnedModelInstance* SkinnedModelInst = nullptr;

    unsigned int InstanceCount = 1;
};

/**
 * \brief render 模型的更新信息通用模型更新，只能改变其中的位置信息
 * 更新其中的transform然后调用Update方法;
 * @see DefaultRenderItemUpdateInfo
 */
struct ModelUpdateInfo
{
    ModelUpdateInfo()
    {
        transform = XMLoadFloat4x4(&MathHelper::Identity4x4()) ;
    }
    void Update();
    int NumFramesDirty = gNumFrameResources2;
    XMMATRIX transform;
};

inline void ModelUpdateInfo::Update()
{
    NumFramesDirty = gNumFrameResources2;
}

struct ModelInstance
{
    int id = -1;
    std::string modelPath;
    MeshGeometry* geometry;
    std::vector<SubMesh> subMeshs;
    std::vector<RenderItem*> subRenderItem = std::vector<RenderItem*>();
    std::shared_ptr<ModelUpdateInfo> modelUpdateInfo =std::make_shared<ModelUpdateInfo>() ;
};

enum class  RenderLayer : int
{
    Opaque = 0,
    SkinnedOpaque,
    Debug,
    Sky,
    Particle,
    Model,
    Count
};
