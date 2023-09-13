#include "RenderPassBase.h"

#include "../../core/macro.h"
#include "../Renderer.h"

ID3D12Resource* RenderPassBase::CurrentBackBuffer() const
{
    return renderer->CurrentBackBuffer();
}

D3D12_CPU_DESCRIPTOR_HANDLE RenderPassBase::CurrentBackBufferView()
{
    return renderer->CurrentBackBufferView();
}

D3D12_CPU_DESCRIPTOR_HANDLE RenderPassBase::DepthStencilView()
{
    return renderer->DepthStencilView();
}

bool RenderPassBase::Initialize(Renderer* _renderer)
{
    mCommandQueue = _renderer->mCommandQueue;
    mDirectCmdListAlloc = _renderer->mDirectCmdListAlloc;
    mCommandList = _renderer->mCommandList;
    md3dDevice = _renderer->md3dDevice;
    mSrvDescriptorHeap = _renderer->mSrvDescriptorHeap;
    renderer = _renderer;
    mPSOs = _renderer->mPSOs;
    mCbvSrvUavDescriptorSize = _renderer->mCbvSrvUavDescriptorSize;
    //waring 
    mScreenViewport = &(_renderer->mScreenViewport);
    mScissorRect = &(_renderer->mScissorRect);
    return true;
}

void DrawRenderItems(FrameResource* mCurrFrameResource, ID3D12GraphicsCommandList* cmdList, const std::vector<RenderItem*>& ritems)
{
    if (mCurrFrameResource == nullptr)
    {
        LOG_WARN("CurrFrameResource == null");
    }
    UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));
    UINT skinnedCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(SkinnedConstants));

    auto objectCB = mCurrFrameResource->ObjectCB->Resource();
    auto skinnedCB = mCurrFrameResource->SkinnedCB->Resource();
    auto instanceCB = mCurrFrameResource->InstanceBuffer->Resource();
    // For each render item...
    for (size_t i = 0; i < ritems.size(); ++i)
    {
        auto ri = ritems[i];
        if(ri->InstanceCount<1)return;
        cmdList->IASetVertexBuffers(0, 1, &(ri->Geo->VertexBufferView()));
        cmdList->IASetIndexBuffer(&ri->Geo->IndexBufferView());
        cmdList->IASetPrimitiveTopology(ri->PrimitiveType);

        D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = objectCB->GetGPUVirtualAddress() + ri->ObjCBIndex * objCBByteSize;
        D3D12_GPU_VIRTUAL_ADDRESS instanceCBAddress = instanceCB->GetGPUVirtualAddress() ;
        cmdList->SetGraphicsRootConstantBufferView(0, objCBAddress);

        if (ri->SkinnedModelInst != nullptr)
        {
            D3D12_GPU_VIRTUAL_ADDRESS skinnedCBAddress = skinnedCB->GetGPUVirtualAddress() + ri->SkinnedCBIndex * skinnedCBByteSize;
            cmdList->SetGraphicsRootConstantBufferView(1, skinnedCBAddress);
        }
        else
        {
            cmdList->SetGraphicsRootConstantBufferView(1, 0);
        }

        if(ri->InstanceCount>1)
        {
            cmdList->SetGraphicsRootShaderResourceView(6, instanceCBAddress);
            cmdList->DrawIndexedInstanced(ri->IndexCount,ri->InstanceCount, ri->StartIndexLocation, ri->BaseVertexLocation, 0);
            
        }
        else
        {
            cmdList->DrawIndexedInstanced(ri->IndexCount, 1, ri->StartIndexLocation, ri->BaseVertexLocation, 0);
        }
        
    }
}
