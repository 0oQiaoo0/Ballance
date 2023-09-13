#pragma once
#include <memory>

#include "Renderer.h"
#include "RenderResource.h"

class Renderer;

// Ҫ��DefaultShapeStr��Ӧ��
enum DefaultShape
{
    Sphere =0,
    Box,
    Cylinder,
    Quad,
    Grid


};
enum DefaultMaterial
{
    Bricks = 0,
    Mirror,
    Tile ,
    Sky,
    Wood,
    Red,// temp ,stone for old
    Paper

};
/**
 * \brief renderĬ��ģ����Ϣ���½ṹ��
 *  ����
 * 1.������Ϣǰ����IsValid()����Ƿ�Ϊ��
 * 2. ���½ṹ����Ϣ
 * 3. ����Update()����
 * \note TexScale ����û��
 * \see  ModelUpdateInfo
 */


class ParticleInstance
{
public:
    friend class RenderScene;
    ParticleInstance(){}
    std::string name;
    std::shared_ptr<std::vector<XMFLOAT4X4>> worldTransform;
    std::shared_ptr<std::vector<XMFLOAT4X4>> TexTransform;
    long count = -1;
    void Update();
private:
    RenderItem* item;
};
class DefaultRenderItemUpdateInfo
{
public:
    friend class RenderScene;
    DefaultShape Shape;
    //no use
    XMFLOAT3 TexScale;
    XMFLOAT4X4 WorldTransform;
    DefaultMaterial Material;
    bool IsValid();
    void Update();
private:
    RenderItem* render_item_ = nullptr;
    RenderScene* scene_ = nullptr;
};
extern const char* DefaultShapeStr[];


class RenderScene
{
    friend class GameEngine;
    friend class Renderer;
    friend class Scene;
public:
    ~RenderScene();
    bool BuildModelMeshInfo(std::string& path);
    RenderScene();
    void BindRendererCamera(Camera* camera);
    void BuildRenderItems();
    bool Initialize();
    void BindRenderer(std::shared_ptr<Renderer> renderer);
    
    /**
     * \brief 
     * \param Shape Ĭ����״����
     * \param WorldTransform �����ʼ�ĳ����任����
     * \param TexScale �����������ţ�������ʹ��
     * \param Material_ Ĭ�ϲ�������
     * \return ���Ը���render���ԵĽṹ��
     * @see DefaultRenderItemUpdateInfo
     */
    DefaultRenderItemUpdateInfo AddDefaultRenderItem(DefaultShape Shape,XMMATRIX WorldTransform,
        XMFLOAT3 TexScale ={10.1f,0.1f,0.1f},
         DefaultMaterial Material_= Wood);
    /**
     * \brief 
     * \param Modelpath ģ��
     * \param WorldTransform 
     * \return ���Ը���render���ԵĽṹ��
     * @see DefaultRenderItemUpdateInfo
     */
    ModelUpdateInfo* AddRenderItem(std::string& Modelpath,
                                   XMMATRIX WorldTransform = XMMatrixTranslationFromVector(
                                       XMVectorSet(1.0f, 2.0f, 3.0f, 0.0f)));


    
    ParticleInstance* AddParticles(DefaultShape Shape, DefaultMaterial Material_, XMMATRIX WorldTransform,
        std::shared_ptr<std::vector<XMFLOAT4X4>> p_worldTransform,
    std::shared_ptr<std::vector<XMFLOAT4X4>> p_TexTransform, long count);
    
    void Tick(float DeltaTime);


    std::unordered_map<std::string, std::shared_ptr<MeshGeometry>>& GetGeometries();

    std::unordered_map<std::string, std::unique_ptr<Material>>& GetMaterials();

    std::vector<std::unique_ptr<RenderItem>>& GetCurrRenderItem();

    float mLightRotationAngle = 0.0f;
    XMFLOAT3 mBaseLightDirections[3] = {
        XMFLOAT3(0.57735f, -0.57735f, 0.57735f),
        XMFLOAT3(-0.57735f, -0.57735f, 0.57735f),
        XMFLOAT3(0.0f, -0.707f, -0.707f)
    };


    float mLightNearZ = 0.0f;
    float mLightFarZ = 0.0f;
    XMFLOAT3 mLightPosW;
    XMFLOAT4X4 mLightView = MathHelper::Identity4x4();
    XMFLOAT4X4 mLightProj = MathHelper::Identity4x4();
    XMFLOAT4X4 mShadowTransform = MathHelper::Identity4x4();

    PassConstants mMainPassCB;  // index 0 of pass cbuffer.
    PassConstants mShadowPassCB;// index 1 of pass cbuffer.
    XMFLOAT3 mRotatedLightDirections[3];
    

    DirectX::BoundingSphere mSceneBounds;
    float totalTime = 0.0f;
protected:
    FrameResource* GetCurrFrameResource();
    void Update(float DeltaTime);
    void UpdateShadowTransform();
    void UpdateShadowPassCB();
    void UpdateParticleCB();
    void UpdateMainPassCB(float deltaTime);
    void UpdateObjectCBs();
    void BuildDynamicShapeGeometry(GeometryGenerator::MeshData meshData);
    void BuildShapeGeometry();
    void BuildMaterials();
    void BuildGpuDefaultBuffer(Microsoft::WRL::ComPtr<ID3D12Resource>& BufferGPU,
        const void* initData,
        UINT64 byteSize,
        Microsoft::WRL::ComPtr<ID3D12Resource>& uploadBuffer);

    Camera* mainCamera = nullptr;

    std::shared_ptr<Renderer> mRenderer;

    std::vector<RenderItem*> mAllRitemLayer[(int)RenderLayer::Count];
    std::vector<RenderItem*> mRitemLayer[(int)RenderLayer::Count];

    std::vector<std::unique_ptr<RenderItem>> CurrRenderItem;

    std::unordered_map<std::string, std::shared_ptr<MeshGeometry>> mGeometries;

    std::unordered_map<std::string, std::unique_ptr<Material>> mMaterials;

    std::unordered_map<std::string, ModelInstance> mModelInstance;

    std::vector<std::shared_ptr< ParticleInstance>> particleInstances;

    
    XMFLOAT3 ShadowCenter = {0,0,0};
    int ObjConstantBufferIndex = 0;
};



inline FrameResource* RenderScene::GetCurrFrameResource()
{
    assert(mRenderer.get()!=nullptr);
    return mRenderer->mCurrFrameResource;
}
