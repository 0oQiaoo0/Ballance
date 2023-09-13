#pragma once
#include "RenderPassBase.h"


struct MainCameraPassInitialInfo
{

};
class MainCameraPass :public RenderPassBase
{
public:
    void Draw(FrameResource* currentResource) override;
    void BuildRootSignature() override;
    void BuildPipelineState() override;
    bool Initialize(Renderer* _renderer) override;
    ID3D12RootSignature* mRootSignature = nullptr;
    UINT mSkyTexHeapIndex = 0;
};
