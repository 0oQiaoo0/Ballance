#pragma once
#include <memory>
#include "../dx12/ShadowMap.h"
#include "RenderPassBase.h"

class ShadowMapPass :public RenderPassBase
{
public:
    bool Initialize(Renderer* _renderer)override ;
    void Draw(FrameResource* currentResource) override;
    void BuildRootSignature() override;
    void BuildPipelineState() override;
    std::unique_ptr<ShadowMap> mShadowMap;
    ID3D12RootSignature* mRootSignature = nullptr;
    CD3DX12_GPU_DESCRIPTOR_HANDLE mNullSrv;
};
