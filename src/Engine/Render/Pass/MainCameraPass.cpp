#include "MainCameraPass.h"

#include "../Renderer.h"


void MainCameraPass::Draw(FrameResource* currentResource)
{
    mCommandList->SetGraphicsRootSignature(mRootSignature);
    auto matBuffer = currentResource->MaterialBuffer->Resource();


    // Rebind state whenever graphics root signature changes.

    // Bind all the materials used in this scene.  For structured buffers, we can bypass the heap and 
    // set as a root descriptor.
    matBuffer = currentResource->MaterialBuffer->Resource();
    mCommandList->SetGraphicsRootShaderResourceView(3, matBuffer->GetGPUVirtualAddress());


    mCommandList->RSSetViewports(1, &*mScreenViewport);
    mCommandList->RSSetScissorRects(1, &*mScissorRect);

    // Indicate a state transition on the resource usage.
    mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
        D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

    // Clear the back buffer and depth buffer.
    mCommandList->ClearRenderTargetView(CurrentBackBufferView(), Colors::LightSteelBlue, 0, nullptr);
    mCommandList->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

    // Specify the buffers we are going to render to.
    mCommandList->OMSetRenderTargets(1, &CurrentBackBufferView(), true, &DepthStencilView());

    // Bind all the textures used in this scene.  Observe
    // that we only have to specify the first descriptor in the table.  
    // The root signature knows how many descriptors are expected in the table.
    mCommandList->SetGraphicsRootDescriptorTable(5, mSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());

    auto passCB = currentResource->PassCB->Resource();
    mCommandList->SetGraphicsRootConstantBufferView(2, passCB->GetGPUVirtualAddress());

    // Bind the sky cube map.  For our demos, we just use one "world" cube map representing the environment
    // from far away, so all objects will use the same cube map and we only need to set it once per-frame.  
    // If we wanted to use "local" cube maps, we would have to change them per-object, or dynamically
    // index into an array of cube maps.

    CD3DX12_GPU_DESCRIPTOR_HANDLE skyTexDescriptor(mSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
    skyTexDescriptor.Offset(mSkyTexHeapIndex, mCbvSrvUavDescriptorSize);
    mCommandList->SetGraphicsRootDescriptorTable(4, skyTexDescriptor);

  
    
    mCommandList->SetPipelineState(mPSOs["opaque"].Get());
    DrawRenderItems(currentResource, mCommandList.Get(), renderer->GetRenderLayer()[(int)RenderLayer::Opaque]);

   
    mCommandList->SetPipelineState(mPSOs["sky"].Get());
    DrawRenderItems(currentResource, mCommandList.Get(), renderer->GetRenderLayer()[(int)RenderLayer::Sky]);


    mCommandList->SetPipelineState(mPSOs["transparent"].Get());
    DrawRenderItems(currentResource, mCommandList.Get(), renderer->GetRenderLayer()[(int)RenderLayer::Particle]);
    //mCommandList->SetPipelineState(mPSOs["skinnedOpaque"].Get());
    //DrawRenderItems(mCurrFrameResource,mCommandList.Get(), mRitemLayer[(int)RenderLayer::SkinnedOpaque]);

    //mCommandList->SetPipelineState(mPSOs["debug"].Get());
    //DrawRenderItems(mCurrFrameResource,mCommandList.Get(), mRitemLayer[(int)RenderLayer::Debug]);

  

}

void MainCameraPass::BuildRootSignature()
{
    mRootSignature = renderer->mRootSignature.Get();
}

void MainCameraPass::BuildPipelineState()
{
}

bool MainCameraPass::Initialize(Renderer* _renderer)
{

    if (RenderPassBase::Initialize(_renderer) == false)
    {
        return false;
    }
    mSkyTexHeapIndex = _renderer->mSkyTexHeapIndex;
    return true;
}
