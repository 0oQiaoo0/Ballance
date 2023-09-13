#pragma once
#include <d3d12.h>
#include <wrl/client.h>
#include "../dx12/FrameResource.h"
#include "../RenderResource.h"

class Renderer;

class RenderPassBase
{
public:
    virtual ~RenderPassBase() = default;
    virtual void Draw(FrameResource* currentResource) = 0;
    virtual void BuildRootSignature() = 0;
    virtual void BuildPipelineState() = 0;
    virtual  bool Initialize(Renderer* _renderer);

protected:

    ID3D12Resource* CurrentBackBuffer() const;
    D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView();
    D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView();

    Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandQueue;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList;

    std::unordered_map<std::string, ComPtr<ID3D12PipelineState>> mPSOs;
    Microsoft::WRL::ComPtr<ID3D12Device> md3dDevice;//

    ComPtr<ID3D12DescriptorHeap> mSrvDescriptorHeap = nullptr;


    D3D12_VIEWPORT* mScreenViewport = nullptr;
    D3D12_RECT* mScissorRect = nullptr;
    UINT mCbvSrvUavDescriptorSize = 0;
    Renderer* renderer = nullptr;

};
void DrawRenderItems(FrameResource* CurrFrameResource, ID3D12GraphicsCommandList* cmdList, const std::vector<RenderItem*>& ritems);