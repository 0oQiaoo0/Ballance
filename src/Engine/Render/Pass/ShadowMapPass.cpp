#include "ShadowMapPass.h"
#include "../Renderer.h"

bool ShadowMapPass::Initialize(Renderer* _renderer)
{
    
    if (RenderPassBase::Initialize(_renderer) == false)
    {
        return false;
    }
    mShadowMap = std::make_unique<ShadowMap>(md3dDevice.Get(),
        4096, 4096);

    return true;
}

void ShadowMapPass::Draw(FrameResource* currentResource)


{
    //
    // Shadow map pass.
    //

    // Bind all the materials used in this scene.  For structured buffers, we can bypass the heap and 
    // set as a root descriptor.
    auto matBuffer = currentResource->MaterialBuffer->Resource();
    mCommandList->SetGraphicsRootShaderResourceView(3, matBuffer->GetGPUVirtualAddress());
	
    // Bind null SRV for shadow map pass
    // .
    mCommandList->SetGraphicsRootDescriptorTable(4, mNullSrv);	 

    // Bind all the textures used in this scene.  Observe
    // that we only have to specify the first descriptor in the table.  
    // The root signature knows how many descriptors are expected in the table.
    mCommandList->SetGraphicsRootDescriptorTable(5, mSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());


    mCommandList->RSSetViewports(1, &mShadowMap->Viewport());
    mCommandList->RSSetScissorRects(1, &mShadowMap->ScissorRect());

    // Change to DEPTH_WRITE.
    mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mShadowMap->Resource(),
        D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE));

    // Clear the back buffer and depth buffer.
    mCommandList->ClearDepthStencilView(mShadowMap->Dsv(),
        D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

    // Specify the buffers we are going to render to.
    mCommandList->OMSetRenderTargets(0, nullptr, false, &mShadowMap->Dsv());

    // Bind the pass constant buffer for the shadow map pass.
    UINT passCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(PassConstants));
    auto passCB = currentResource->PassCB->Resource();
    D3D12_GPU_VIRTUAL_ADDRESS passCBAddress = passCB->GetGPUVirtualAddress() + 1*passCBByteSize;
    mCommandList->SetGraphicsRootConstantBufferView(2, passCBAddress);

    mCommandList->SetPipelineState(mPSOs["shadow_opaque"].Get());
    DrawRenderItems(currentResource, mCommandList.Get(), renderer->GetRenderLayer()[(int)RenderLayer::Opaque]);

    mCommandList->SetPipelineState(mPSOs["skinnedShadow_opaque"].Get());
    DrawRenderItems(currentResource, mCommandList.Get(), renderer->GetRenderLayer()[(int)RenderLayer::SkinnedOpaque]);
    // Change back to GENERIC_READ so we can read the texture in a shader.
    mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mShadowMap->Resource(),
        D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_GENERIC_READ));

}

void ShadowMapPass::BuildRootSignature()
{
    mRootSignature = renderer->mRootSignature.Get();
}

void ShadowMapPass::BuildPipelineState()
{
}
