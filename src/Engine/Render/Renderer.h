#pragma once
#include <d3d9.h>
#include "dx12/Common/Camera.h"
#include "dx12/FrameResource.h"
#include "dx12/Common/d3dUtil.h"
#include "Pass/RenderPassBase.h"
#include <functional>

#include "Pass/MainCameraPass.h"
#include "Pass/ShadowMapPass.h"
#include "Pass/UIPass.h"

class RenderScene;
LPCWSTR ConvertToLPCWSTR(const char* str);
class Renderer;
class Renderer
{
	friend class RenderScene;
	friend class GameEngine;
	friend class UISystem;
	friend class RenderPassBase;

public:
	Renderer();
	~Renderer();
	std::vector<std::unique_ptr<RenderItem>>& GetRenderItem();
	std::unordered_map<std::string, std::unique_ptr<Material>>& GetMaterials();
	std::vector<RenderItem*>* GetRenderLayer();
	void BuildGpuDefaultBuffer(Microsoft::WRL::ComPtr<ID3D12Resource>& BufferGPU,
		const void* initData,
		UINT64 byteSize,
		Microsoft::WRL::ComPtr<ID3D12Resource>& uploadBuffer);
	std::shared_ptr<ShadowMapPass> ShadowPass;
	std::shared_ptr<UIPass> uiPass;
	std::shared_ptr<MainCameraPass> mainCameraPass;
	bool InitMainWindow();
	void SetFullScreen(bool full); // 全屏显示
	bool Initialize(RenderScene* renderScene);
	//void OnKeyboardInput(float DeltaTime);
	Camera* mCamera;
	void OnResize();
	void Update(float DeltaTime);
	//void UpdateMainPassCB();
	void UpdateMaterialBuffer();
	//void UpdateObjectCBs();
	void DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<RenderItem*>& ritems);
	void Draw(float DeltaTime);
	LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	static  Renderer* GetRenderer();

	inline  HWND GetHMainWnd() { return mhMainWnd; }
	inline  ID3D12Device* Getd3dDevice() { return md3dDevice.Get(); }


	ID3D12Resource* CurrentBackBuffer()const;
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView()const;
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView()const;
	bool InitPass();

	bool InitDirect3D();
	void CreateCommandObjects();
	void CreateSwapChain();
	void CreateRtvAndDsvDescriptorHeaps();
	void BuildShadersAndInputLayout();

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDsvHeap;
	void FlushCommandQueue();
	void FlushFrameResourceQueue();

	int mMatCBIndex = 8;
	void LoadSkinnedModel();
	void CreateDynamicModel(std::string& path,  ModelInstance& modelInstance);
	void LoadModel(std::string& path, std::string& MeshName);
	void LoadModel(std::string& path,  ModelInstance& modelInstance);
	void LoadTextures();
	void BuildRootSignature();
	void BuildSsaoRootSignature();
	void BuildDescriptorHeaps();
	//void UpdateShadowTransform();
	//void UpdateShadowPassCB();
	void BuildFrameResources();
	void BuildPSOs();
	std::array<const CD3DX12_STATIC_SAMPLER_DESC, 7> Renderer::GetStaticSamplers();

	HINSTANCE mhAppInst = nullptr; // application instance handle
	HWND      mhMainWnd = nullptr; // main window handle
	bool      mAppPaused = false;  // is the application paused?
	bool      mMinimized = false;  // is the application minimized?
	bool      mMaximized = false;  // is the application maximized?
	bool      mResizing = false;   // are the resize bars being dragged?
	bool      mFullscreenState = false;// fullscreen enabled

	// Set true to use 4X MSAA (?.1.8).  The default is false.
	bool      m4xMsaaState = false;    // 4X MSAA enabled
	UINT      m4xMsaaQuality = 0;      // quality level of 4X MSAA

	// Used to keep track of the Delta-time?and game time (?.4).

	Microsoft::WRL::ComPtr<IDXGIFactory4> mdxgiFactory;
	Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
	Microsoft::WRL::ComPtr<ID3D12Device> md3dDevice;//设备

	Microsoft::WRL::ComPtr<ID3D12Fence> mFence;//用于同步
	UINT64 mCurrentFence = 0;

	Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandQueue;//指令队列
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList;//指令列表

	static const int SwapChainBufferCount = 2;
	int mCurrBackBuffer = 0;
	Microsoft::WRL::ComPtr<ID3D12Resource> mSwapChainBuffer[SwapChainBufferCount];
	Microsoft::WRL::ComPtr<ID3D12Resource> mDepthStencilBuffer;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mRtvHeap;

	ComPtr<ID3D12DescriptorHeap> mSrvDescriptorHeap = nullptr;

	std::unordered_map<std::string, ComPtr<ID3D12PipelineState>> mPSOs;

	ComPtr<ID3D12RootSignature> mRootSignature = nullptr;
	ComPtr<ID3D12RootSignature> mSsaoRootSignature = nullptr;
	D3D12_VIEWPORT mScreenViewport;
	D3D12_RECT mScissorRect;

	UINT mRtvDescriptorSize = 0;
	UINT mDsvDescriptorSize = 0;
	UINT mCbvSrvUavDescriptorSize = 0;

	// Derived class should set these in derived constructor to customize starting values.
	std::string mMainWndCaption = "d3d App";
	D3D_DRIVER_TYPE md3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
	DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	int mClientWidth = 800;
	int mClientHeight = 600;


	std::unordered_map<std::string, std::unique_ptr<Texture>> mTextures;


	UINT mSkyTexHeapIndex = 0;
	UINT mShadowMapHeapIndex = 0;
	UINT mSsaoHeapIndexStart = 0;
	UINT mSsaoAmbientMapIndex = 0;

	UINT mNullCubeSrvIndex = 0;
	UINT mNullTexSrvIndex1 = 0;
	UINT mNullTexSrvIndex2 = 0;
	UINT mDynamicTexSrvIndex = 0;
	CD3DX12_GPU_DESCRIPTOR_HANDLE mNullSrv;
	//std::unique_ptr<ShadowMap> mShadowMap;

	CD3DX12_CPU_DESCRIPTOR_HANDLE GetCpuSrv(int index)const;
	CD3DX12_GPU_DESCRIPTOR_HANDLE GetGpuSrv(int index)const;
	CD3DX12_CPU_DESCRIPTOR_HANDLE GetDsv(int index)const;
	CD3DX12_CPU_DESCRIPTOR_HANDLE GetRtv(int index)const;


	std::unordered_map<std::string, ComPtr<ID3DBlob>> mShaders;
	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;
	std::vector<D3D12_INPUT_ELEMENT_DESC> mSkinnedInputLayout;
	//std::unordered_map<std::string, std::unique_ptr<MeshGeometry>> mGeometries;

	//std::unordered_map<std::string, std::unique_ptr<Material>> mMaterials;

	// List of all the render items.
	//std::vector<std::unique_ptr<RenderItem>> mAllRitems;

	// Render items divided by PSO.
	//std::vector<RenderItem*> mRitemLayer[(int)RenderLayer::Count];



	std::vector<std::unique_ptr<FrameResource>> mFrameResources;


	std::unique_ptr<Ssao> mSsao;

	UINT mSkinnedSrvHeapStart = 0;
	std::string mSkinnedModelFilename = "..\\..\\src\\Asset\\Models\\soldier.m3d";
	std::unique_ptr<SkinnedModelInstance> mSkinnedModelInst;
	SkinnedData mSkinnedInfo;
	std::vector<M3DLoader::Subset> mSkinnedSubsets;
	std::vector<M3DLoader::M3dMaterial> mSkinnedMats;
	std::vector<std::string> mSkinnedTextureNames;
	

	RenderScene* renderScene;

private:
	FrameResource* mCurrFrameResource = nullptr;
	int mCurrFrameResourceIndex = 0;
	static Renderer* mRenderer;
	void LogAdapters();
	void LogAdapterOutputs(IDXGIAdapter* adapter);
	void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);
};


