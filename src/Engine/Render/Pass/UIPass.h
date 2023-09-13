#pragma once
#include <functional>

#include "RenderPassBase.h"

struct ImageInfo
{
    ImageInfo(D3D12_GPU_DESCRIPTOR_HANDLE image_id, unsigned width, unsigned height)
        : ImageId(image_id),
        Width(width),
        Height(height)
    {
    }

    D3D12_GPU_DESCRIPTOR_HANDLE ImageId = D3D12_GPU_DESCRIPTOR_HANDLE{ NULL };
    unsigned int Width = -1;
    unsigned int Height = -1;
};

class UIPass final : public RenderPassBase
{
public:
    ~UIPass();
    void Draw(FrameResource* currentResource) override;
    bool Initialize(Renderer* _renderer) override;
    void BuildRootSignature() override;
    void BuildPipelineState() override;
    void BuildImageDescriptor(const std::vector<std::string>& path, std::vector<ImageInfo>& OutInfo);
    ID3D12DescriptorHeap* g_pd3dSrvDescHeap = nullptr;
    std::function<void()> UiDrawBack;


protected:
    bool LoadTextureFromFile(const char* filename, ID3D12Device* d3d_device, D3D12_CPU_DESCRIPTOR_HANDLE srv_cpu_handle,
        ID3D12Resource** out_tex_resource, int* out_width, int* out_height);
    std::vector<ID3D12Resource*> Resources;
};

