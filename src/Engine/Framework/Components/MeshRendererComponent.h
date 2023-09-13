#pragma once
#include "../Component.h"

#include <string>

#include "../../Render/RenderScene.h"

struct RenderInfo
{
    DirectX::XMVECTORF32 Position;
    DirectX::XMVECTORF32 Scale;
    DirectX::XMVECTORF32 Rotation;
};

class TransformComponent;
struct RenderItem;
class MeshRendererComponent :public Component
{
private:
    TransformComponent* transformComponent;
   
    DefaultShape mShape;
    std::string mShapeStr = "";
    std::string mModelPath = "";
protected:
    void Update(float DeltaTime) override;
public:
    DefaultRenderItemUpdateInfo renderItem;
    ModelUpdateInfo* renderModelItem=nullptr;
    void Start() override;
    /**
     * \brief 生成一个自定义形状到渲染场景中，未完善
     * \param Shape 形状
     * \param TexScale 纹理缩放
     * \param WorldTransform 世界变换矩阵
     * \return
     */
    void RenderDefaultShape(DefaultShape Shape, XMFLOAT3 TexScale = XMFLOAT3(1.0, 1.0, 1.0));
    void RenderModel(std::string modelPath);
    void ShowDetailUI() override;
    void PretendRemove();
};
