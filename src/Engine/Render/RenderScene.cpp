#include "RenderScene.h"

#include <map>
#include <memory>
#include <memory>

#include "Renderer.h"
#include "../core/macro.h"
#include "../Input/InputSystem.h"


#define ENUM_SHAPE(x) DefaultShapeStr[x]
const char* DefaultShapeStr[] = {
	"Sphere",
	"Box",
	"Cylinder",
	"Quad",
	"Grid"

};
const char* DefaultMaterialStr[] = {
	"bricks0",
	"mirror0",
	"tile0",
	"sky",
	"wood0",
	"stone0",
	"paper0",
};

void ParticleInstance::Update()
{
	item->NumFramesDirty = gNumFrameResources2;
}

bool DefaultRenderItemUpdateInfo::IsValid()
{
	return scene_!=nullptr&&render_item_!=nullptr;
}

void DefaultRenderItemUpdateInfo::Update()
{	
	if(render_item_==nullptr||scene_==nullptr)return;
	MeshGeometry * geo = render_item_->Geo;
	render_item_->IndexCount = geo->DrawArgs[ENUM_SHAPE(Shape)].IndexCount;
	render_item_->StartIndexLocation = geo->DrawArgs[ENUM_SHAPE(Shape)].StartIndexLocation;
	render_item_->BaseVertexLocation = geo->DrawArgs[ENUM_SHAPE(Shape)].BaseVertexLocation;
	render_item_->World = WorldTransform;
	render_item_->NumFramesDirty = gNumFrameResources2;
	render_item_->Mat = scene_->GetMaterials()[DefaultMaterialStr[Material]].get();
	//XMStoreFloat4x4(&render_item_->TexTransform, XMMatrixScaling(TexScale.x, TexScale.y, TexScale.z));
}

RenderScene::~RenderScene()
{
	std::cout << "~RenderScene" << std::endl;
	if (mRenderer)
	{
		mRenderer->FlushCommandQueue();
	}
}

bool RenderScene::BuildModelMeshInfo(std::string& path)
{
	if(mModelInstance.count(path)>0)
	{
		LOG_WARN("model "+path+" has already load");
		return false;
	}
	ModelInstance NewModelMeshInfo;
	mRenderer->	LoadModel(path,NewModelMeshInfo);
	mModelInstance[path] = NewModelMeshInfo;
	return true;
}

RenderScene::RenderScene()
{
	//mainCamera = std::make_shared<Camera>();
	//mainCamera->SetPosition(0.0f, 2.0f, -15.0f);
	mSceneBounds.Center = XMFLOAT3(0.0f, 0.0f, 0.0f);
	mSceneBounds.Radius = sqrtf(10.0f*10.0f + 15.0f*15.0f);
}
void RenderScene::BindRendererCamera(Camera* camera)
{
	mRenderer->mCamera = camera;
	mainCamera = camera;
	mRenderer->OnResize();
}
void RenderScene::BuildRenderItems()
{
	auto skyRitem = std::make_unique<RenderItem>();
	XMStoreFloat4x4(&skyRitem->World, XMMatrixScaling(5000.0f, 5000.0f, 5000.0f));
	skyRitem->TexTransform = MathHelper::Identity4x4();
	skyRitem->ObjCBIndex = ObjConstantBufferIndex++;
	skyRitem->Mat = mMaterials["sky"].get();
	skyRitem->Geo = mGeometries["shapeGeo"].get();
	skyRitem->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	skyRitem->IndexCount = skyRitem->Geo->DrawArgs["Sphere"].IndexCount;
	skyRitem->StartIndexLocation = skyRitem->Geo->DrawArgs["Sphere"].StartIndexLocation;
	skyRitem->BaseVertexLocation = skyRitem->Geo->DrawArgs["Sphere"].BaseVertexLocation;

	mRitemLayer[(int)RenderLayer::Sky].push_back(skyRitem.get());
	CurrRenderItem.push_back(std::move(skyRitem));
	
	/*
	auto quadRitem = std::make_unique<RenderItem>();
	quadRitem->World = MathHelper::Identity4x4();
	quadRitem->TexTransform = MathHelper::Identity4x4();
	quadRitem->ObjCBIndex = 1;
	quadRitem->Mat = mMaterials["bricks0"].get();
	quadRitem->Geo = mGeometries["shapeGeo"].get();
	quadRitem->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	quadRitem->IndexCount = quadRitem->Geo->DrawArgs["quad"].IndexCount;
	quadRitem->StartIndexLocation = quadRitem->Geo->DrawArgs["quad"].StartIndexLocation;
	quadRitem->BaseVertexLocation = quadRitem->Geo->DrawArgs["quad"].BaseVertexLocation;

	mRitemLayer[(int)RenderLayer::Debug].push_back(quadRitem.get());
	CurrRenderItem.push_back(std::move(quadRitem));

	auto boxRitem1 = std::make_unique<RenderItem>();
	XMStoreFloat4x4(&boxRitem1->World, XMMatrixScaling(2.0f, 1.0f, 2.0f)*XMMatrixTranslation(10.0f, 0.5f, 0.0f));
	XMStoreFloat4x4(&boxRitem1->TexTransform, XMMatrixScaling(1.0f, 1.0f, 1.0f));
	boxRitem1->ObjCBIndex = 2;
	boxRitem1->Mat = mMaterials["bricks0"].get();
	boxRitem1->Geo = mGeometries["shapeGeo"].get();
	boxRitem1->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	boxRitem1->IndexCount = boxRitem1->Geo->DrawArgs["box"].IndexCount;
	boxRitem1->StartIndexLocation = boxRitem1->Geo->DrawArgs["box"].StartIndexLocation;
	boxRitem1->BaseVertexLocation = boxRitem1->Geo->DrawArgs["box"].BaseVertexLocation;


	mRitemLayer[(int)RenderLayer::Opaque].push_back(boxRitem1.get());
	CurrRenderItem.push_back(std::move(boxRitem1));
	auto boxRitem2 = std::make_unique<RenderItem>();
	XMStoreFloat4x4(&boxRitem2->World, XMMatrixScaling(2.0f, 2.0f, 2.0f)*XMMatrixTranslation(0.0f, 0.5f, 0.0f));
	XMStoreFloat4x4(&boxRitem2->TexTransform, XMMatrixScaling(1.0f, 1.0f, 1.0f));
	boxRitem2->ObjCBIndex = 3;
	boxRitem2->Mat = mMaterials["mirror0"].get();
	boxRitem2->Geo = mGeometries["shapeGeo"].get();
	boxRitem2->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	boxRitem2->IndexCount = boxRitem2->Geo->DrawArgs["sphere"].IndexCount;
	boxRitem2->StartIndexLocation = boxRitem2->Geo->DrawArgs["sphere"].StartIndexLocation;
	boxRitem2->BaseVertexLocation = boxRitem2->Geo->DrawArgs["sphere"].BaseVertexLocation;

	mRitemLayer[(int)RenderLayer::Opaque].push_back(boxRitem2.get());
	CurrRenderItem.push_back(std::move(boxRitem2));
	*/
}

bool RenderScene::Initialize()
{
	mRenderer->Initialize(this);
	return true;
}

void RenderScene::BindRenderer(std::shared_ptr<Renderer> renderer)
{
	mRenderer = renderer;
	mRenderer->mCamera = mainCamera;
}

DefaultRenderItemUpdateInfo RenderScene::AddDefaultRenderItem(DefaultShape Shape,XMMATRIX WorldTransform,
	XMFLOAT3 TexScale,DefaultMaterial Material_)
{
	auto boxRitem = std::make_unique<RenderItem>();
	XMStoreFloat4x4(&boxRitem->World, WorldTransform);
	XMStoreFloat4x4(&boxRitem->TexTransform, XMMatrixScaling(TexScale.x, TexScale.y, TexScale.z));
	boxRitem->ObjCBIndex = ObjConstantBufferIndex++;
	boxRitem->Mat = mMaterials[DefaultMaterialStr[Material_]].get();
	boxRitem->Geo = mGeometries["shapeGeo"].get();
	boxRitem->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	DirectX::BoundingBox 	bounding_box = BoundingBox(XMFLOAT3(0,0,0),XMFLOAT3(1,1,1));

	boxRitem->IndexCount = boxRitem->Geo->DrawArgs[DefaultShapeStr[Shape]].IndexCount;
	boxRitem->StartIndexLocation = boxRitem->Geo->DrawArgs[DefaultShapeStr[Shape]].StartIndexLocation;
	boxRitem->BaseVertexLocation = boxRitem->Geo->DrawArgs[DefaultShapeStr[Shape]].BaseVertexLocation;
	boxRitem->boundingBox=bounding_box;
	mAllRitemLayer[(int)RenderLayer::Opaque].push_back(boxRitem.get());
	CurrRenderItem.push_back(std::move(boxRitem));

	
	DefaultRenderItemUpdateInfo update_info;
	update_info.render_item_ = mAllRitemLayer[(int)RenderLayer::Opaque].back();
	update_info.scene_ = this;
	update_info.Material =Material_;
	update_info.Shape = Shape;
	update_info.TexScale =TexScale;
	update_info.WorldTransform = update_info.render_item_->World;
	return update_info;
}

ModelUpdateInfo* RenderScene::AddRenderItem(std::string& Modelpath, XMMATRIX WorldTransform)
{
	if( mModelInstance.count(Modelpath)<=0){
		BuildModelMeshInfo(Modelpath);
		LOG_ERROR(" Model "+Modelpath+" not found ,have you call BuildMeshInfo()?");
	}

	ModelInstance& model = mModelInstance[Modelpath];

	ModelInstance modelNew;
	modelNew.modelPath = Modelpath;
	modelNew.id = mModelInstance.size();
	modelNew.geometry = model.geometry;
	for(auto& submesh:model.subMeshs)
	{
		std::unique_ptr<RenderItem> ritem = std::make_unique<RenderItem>();
		XMStoreFloat4x4(&ritem->World, WorldTransform);

		ritem->TexTransform = MathHelper::Identity4x4();
		ritem->ObjCBIndex =ObjConstantBufferIndex++;
		ritem->Mat = submesh.material;//mMaterials["myMaterial"].get() ;//
		ritem->Geo = model.geometry;
		ritem->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		ritem->IndexCount = submesh.IndexCount;
		ritem->StartIndexLocation =submesh.StartIndexLocation;
		ritem->BaseVertexLocation =submesh.BaseVertexLocation;

		// All render items for this solider.m3d instance share
		// the same skinned model instance.
		ritem->SkinnedCBIndex = 0;
		ritem->SkinnedModelInst = nullptr;
		ritem->boundingBox = submesh.Bounds;
		modelNew.subRenderItem.push_back(ritem.get());
		mAllRitemLayer[(int)RenderLayer::Model].push_back(ritem.get());
		CurrRenderItem.push_back(std::move(ritem));
	}
	mModelInstance[Modelpath+std::to_string(modelNew.id)]= modelNew;
	modelNew.modelUpdateInfo->transform = WorldTransform;
	return modelNew.modelUpdateInfo.get(); 
}
//WorldTransform will be overwrited
ParticleInstance* RenderScene::AddParticles(DefaultShape Shape,DefaultMaterial Material_, XMMATRIX WorldTransform,
	std::shared_ptr<std::vector<XMFLOAT4X4>> p_worldTransform,
	std::shared_ptr<std::vector<XMFLOAT4X4>> p_TexTransform,
	long count)
{
	std::shared_ptr<ParticleInstance> particle_instance  = std::make_shared<ParticleInstance>() ;
	particle_instance->count = count;
	particle_instance->worldTransform = p_worldTransform;
	particle_instance->TexTransform = p_TexTransform;
	
	auto particles = std::make_unique<RenderItem>();
	XMStoreFloat4x4(&particles->World,   WorldTransform);
	XMStoreFloat4x4(&particles->TexTransform, XMMatrixIdentity());
	
	particles->ObjCBIndex = ObjConstantBufferIndex++;
	particles->Mat = mMaterials[DefaultMaterialStr[Material_]].get();
	particles->Geo = mGeometries["shapeGeo"].get();
	particles->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	DirectX::BoundingBox 	bounding_box = BoundingBox(XMFLOAT3(0,0,0),XMFLOAT3(1,1,1));

	particles->IndexCount = particles->Geo->DrawArgs[DefaultShapeStr[Shape]].IndexCount;
	particles->StartIndexLocation = particles->Geo->DrawArgs[DefaultShapeStr[Shape]].StartIndexLocation;
	particles->BaseVertexLocation = particles->Geo->DrawArgs[DefaultShapeStr[Shape]].BaseVertexLocation;
	particles->boundingBox=bounding_box;
	particles->InstanceCount = count;
	particleInstances.push_back(particle_instance);
	mRitemLayer[(int)RenderLayer::Particle].push_back(particles.get());
	CurrRenderItem.push_back(std::move(particles));
	particle_instance->item = CurrRenderItem.back().get();
	return particleInstances.back().get();
	
}
void RenderScene::UpdateObjectCBs()
{
	auto currObjectCB = GetCurrFrameResource()->ObjectCB.get();
	for(auto& ModelInstance:mModelInstance)
	{
		ModelUpdateInfo* updateInfo = ModelInstance.second.modelUpdateInfo.get();
		if(updateInfo->NumFramesDirty<=0)continue;
		for(RenderItem* renderitem:ModelInstance.second.subRenderItem)
		{
			
				XMMATRIX world = XMLoadFloat4x4(&renderitem->World);
				//world transform 
				world = updateInfo->transform;
				//
				XMMATRIX texTransform = XMLoadFloat4x4(&renderitem->TexTransform);

				ObjectConstants objConstants;
				XMStoreFloat4x4(&objConstants.World, XMMatrixTranspose(world));
				XMStoreFloat4x4(&objConstants.TexTransform, XMMatrixTranspose(texTransform));
				objConstants.MaterialIndex = renderitem->Mat->MatCBIndex;
				currObjectCB->CopyData(renderitem->ObjCBIndex, objConstants);
				// Next FrameResource need to be updated too.
			}
		updateInfo->NumFramesDirty--;
		}
	
	
	for (RenderItem* e : mAllRitemLayer[(int)RenderLayer::Opaque])
	{
		// Only update the cbuffer data if the constants have changed.  
		// This needs to be tracked per frame resource.
		if (e->NumFramesDirty > 0)
		{
			XMMATRIX world = XMLoadFloat4x4(&e->World);
			XMMATRIX texTransform = XMLoadFloat4x4(&e->TexTransform);

			ObjectConstants objConstants;
			XMStoreFloat4x4(&objConstants.World, XMMatrixTranspose(world));
			XMStoreFloat4x4(&objConstants.TexTransform, XMMatrixTranspose(texTransform));
			objConstants.MaterialIndex = e->Mat->MatCBIndex;

			currObjectCB->CopyData(e->ObjCBIndex, objConstants);

			// Next FrameResource need to be updated too.
			e->NumFramesDirty--;
		}
	}
	for (RenderItem* e : mRitemLayer[(int)RenderLayer::Sky])
	{
		// Only update the cbuffer data if the constants have changed.  
		// This needs to be tracked per frame resource.
		if (e->NumFramesDirty > 0)
		{
			XMMATRIX world = XMLoadFloat4x4(&e->World);
			XMMATRIX texTransform = XMLoadFloat4x4(&e->TexTransform);

			ObjectConstants objConstants;
			XMStoreFloat4x4(&objConstants.World, XMMatrixTranspose(world));
			XMStoreFloat4x4(&objConstants.TexTransform, XMMatrixTranspose(texTransform));
			objConstants.MaterialIndex = e->Mat->MatCBIndex;

			currObjectCB->CopyData(e->ObjCBIndex, objConstants);

			// Next FrameResource need to be updated too.
			e->NumFramesDirty--;
		}
	}
}
/*void RenderScene::BuildDynamicShapeGeometry(GeometryGenerator::MeshData meshData )
{
	ModelInstance modelInstance;
	std::vector<Vertex> vertices(meshData.Vertices.size());

	UINT k = 0;
	for (size_t i = 0; i < box.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box.Vertices[i].Position;
		vertices[k].Normal = box.Vertices[i].Normal;
		vertices[k].TexC = box.Vertices[i].TexC;
		vertices[k].TangentU = box.Vertices[i].TangentU;
	}
	auto geo = std::make_shared<MeshGeometry>();
	geo->Name = "DynamicShapeGeo";

	ThrowIfFailed(D3DCreateBlob(meshData.Vertices.size(), &geo->VertexBufferCPU));
	//CopyMemory(geo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

	ThrowIfFailed(D3DCreateBlob(ibByteSize, &geo->IndexBufferCPU));
	//CopyMemory(geo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

	BuildGpuDefaultBuffer(geo->VertexBufferGPU, meshData.Vertices.data(), meshData.Vertices.size(), geo->VertexBufferUploader);

	BuildGpuDefaultBuffer(geo->IndexBufferGPU, meshData.Indices32.data(), meshData.Indices32.size(), geo->IndexBufferUploader);

	geo->VertexByteStride = sizeof(Vertex);
	geo->VertexBufferByteSize = meshData.Vertices.size();
	geo->IndexFormat = DXGI_FORMAT_R16_UINT;
	geo->IndexBufferByteSize = meshData.Indices32.size();

	SubMesh subMesh;
	subMesh.material = nullptr;
	subMesh.StartIndexLocation = 0;
	subMesh.StartIndexLocation = 0;
	subMesh.IndexCount = meshData.Indices32.size();
	modelInstance.subMeshs.push_back(std::move(subMesh));

	auto render_item = std::make_unique<RenderItem>();
	XMStoreFloat4x4(&render_item->World, WorldTransform);
	XMStoreFloat4x4(&render_item->TexTransform, XMMatrixScaling(TexScale.x, TexScale.y, TexScale.z));
	render_item->ObjCBIndex = ObjConstantBufferIndex++;
	render_item->Mat = mMaterials[DefaultMaterialStr[Material_]].get();
	render_item->Geo = mGeometries["shapeGeo"].get();
	render_item->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	DirectX::BoundingBox 	bounding_box = BoundingBox(XMFLOAT3(0,0,0),XMFLOAT3(1,1,1));

	render_item->IndexCount = render_item->Geo->DrawArgs[DefaultShapeStr[Shape]].IndexCount;
	render_item->StartIndexLocation = render_item->Geo->DrawArgs[DefaultShapeStr[Shape]].StartIndexLocation;
	render_item->BaseVertexLocation = render_item->Geo->DrawArgs[DefaultShapeStr[Shape]].BaseVertexLocation;
	render_item->boundingBox=bounding_box;
	mAllRitemLayer[(int)RenderLayer::Opaque].push_back(render_item.get());
	CurrRenderItem.push_back(std::move(render_item));
	
	mGeometries[geo->Name] = std::move(geo);
}*/
void RenderScene::BuildShapeGeometry()
{
	GeometryGenerator geoGen;
	GeometryGenerator::MeshData box = geoGen.CreateBox(1.0f, 1.0f, 1.0f, 3);
	GeometryGenerator::MeshData grid = geoGen.CreateGrid(1.0f, 1.0f, 4, 4);
	GeometryGenerator::MeshData sphere = geoGen.CreateSphere(0.5f, 20, 20);
	GeometryGenerator::MeshData cylinder = geoGen.CreateCylinder(0.5f, 0.3f, 3.0f, 20, 20);
	GeometryGenerator::MeshData quad = geoGen.CreateQuad(0.0f, 0.0f, 1.0f, 1.0f, 0.0f);

	//
	// We are concatenating all the geometry into one big vertex/index buffer.  So
	// define the regions in the buffer each submesh covers.
	//

	// Cache the vertex offsets to each object in the concatenated vertex buffer.
	UINT boxVertexOffset = 0;
	UINT gridVertexOffset = (UINT)box.Vertices.size();
	UINT sphereVertexOffset = gridVertexOffset + (UINT)grid.Vertices.size();
	UINT cylinderVertexOffset = sphereVertexOffset + (UINT)sphere.Vertices.size();
	UINT quadVertexOffset = cylinderVertexOffset + (UINT)cylinder.Vertices.size();

	// Cache the starting index for each object in the concatenated index buffer.
	UINT boxIndexOffset = 0;
	UINT gridIndexOffset = (UINT)box.Indices32.size();
	UINT sphereIndexOffset = gridIndexOffset + (UINT)grid.Indices32.size();
	UINT cylinderIndexOffset = sphereIndexOffset + (UINT)sphere.Indices32.size();
	UINT quadIndexOffset = cylinderIndexOffset + (UINT)cylinder.Indices32.size();

	SubmeshGeometry boxSubmesh;
	boxSubmesh.IndexCount = (UINT)box.Indices32.size();
	boxSubmesh.StartIndexLocation = boxIndexOffset;
	boxSubmesh.BaseVertexLocation = boxVertexOffset;

	SubmeshGeometry gridSubmesh;
	gridSubmesh.IndexCount = (UINT)grid.Indices32.size();
	gridSubmesh.StartIndexLocation = gridIndexOffset;
	gridSubmesh.BaseVertexLocation = gridVertexOffset;

	SubmeshGeometry sphereSubmesh;
	sphereSubmesh.IndexCount = (UINT)sphere.Indices32.size();
	sphereSubmesh.StartIndexLocation = sphereIndexOffset;
	sphereSubmesh.BaseVertexLocation = sphereVertexOffset;

	SubmeshGeometry cylinderSubmesh;
	cylinderSubmesh.IndexCount = (UINT)cylinder.Indices32.size();
	cylinderSubmesh.StartIndexLocation = cylinderIndexOffset;
	cylinderSubmesh.BaseVertexLocation = cylinderVertexOffset;

	SubmeshGeometry quadSubmesh;
	quadSubmesh.IndexCount = (UINT)quad.Indices32.size();
	quadSubmesh.StartIndexLocation = quadIndexOffset;
	quadSubmesh.BaseVertexLocation = quadVertexOffset;

	//
	// Extract the vertex elements we are interested in and pack the
	// vertices of all the meshes into one vertex buffer.
	//

	auto totalVertexCount =
		box.Vertices.size() +
		grid.Vertices.size() +
		sphere.Vertices.size() +
		cylinder.Vertices.size() +
		quad.Vertices.size();

	std::vector<Vertex> vertices(totalVertexCount);

	UINT k = 0;
	for (size_t i = 0; i < box.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box.Vertices[i].Position;
		vertices[k].Normal = box.Vertices[i].Normal;
		vertices[k].TexC = box.Vertices[i].TexC;
		vertices[k].TangentU = box.Vertices[i].TangentU;
	}

	for (size_t i = 0; i < grid.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = grid.Vertices[i].Position;
		vertices[k].Normal = grid.Vertices[i].Normal;
		vertices[k].TexC = grid.Vertices[i].TexC;
		vertices[k].TangentU = grid.Vertices[i].TangentU;
	}

	for (size_t i = 0; i < sphere.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = sphere.Vertices[i].Position;
		vertices[k].Normal = sphere.Vertices[i].Normal;
		vertices[k].TexC = sphere.Vertices[i].TexC;
		vertices[k].TangentU = sphere.Vertices[i].TangentU;
	}

	for (size_t i = 0; i < cylinder.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = cylinder.Vertices[i].Position;
		vertices[k].Normal = cylinder.Vertices[i].Normal;
		vertices[k].TexC = cylinder.Vertices[i].TexC;
		vertices[k].TangentU = cylinder.Vertices[i].TangentU;
	}

	for (int i = 0; i < quad.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = quad.Vertices[i].Position;
		vertices[k].Normal = quad.Vertices[i].Normal;
		vertices[k].TexC = quad.Vertices[i].TexC;
		vertices[k].TangentU = quad.Vertices[i].TangentU;
	}

	std::vector<std::uint16_t> indices;
	indices.insert(indices.end(), std::begin(box.GetIndices16()), std::end(box.GetIndices16()));
	indices.insert(indices.end(), std::begin(grid.GetIndices16()), std::end(grid.GetIndices16()));
	indices.insert(indices.end(), std::begin(sphere.GetIndices16()), std::end(sphere.GetIndices16()));
	indices.insert(indices.end(), std::begin(cylinder.GetIndices16()), std::end(cylinder.GetIndices16()));
	indices.insert(indices.end(), std::begin(quad.GetIndices16()), std::end(quad.GetIndices16()));

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

	auto geo = std::make_shared<MeshGeometry>();
	geo->Name = "shapeGeo";

	ThrowIfFailed(D3DCreateBlob(vbByteSize, &geo->VertexBufferCPU));
	//CopyMemory(geo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

	ThrowIfFailed(D3DCreateBlob(ibByteSize, &geo->IndexBufferCPU));
	//CopyMemory(geo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

	BuildGpuDefaultBuffer(geo->VertexBufferGPU, vertices.data(), vbByteSize, geo->VertexBufferUploader);

	BuildGpuDefaultBuffer(geo->IndexBufferGPU, indices.data(), ibByteSize, geo->IndexBufferUploader);

	geo->VertexByteStride = sizeof(Vertex);
	geo->VertexBufferByteSize = vbByteSize;
	geo->IndexFormat = DXGI_FORMAT_R16_UINT;
	geo->IndexBufferByteSize = ibByteSize;

	geo->DrawArgs["Box"] = boxSubmesh;
	geo->DrawArgs["Grid"] = gridSubmesh;
	geo->DrawArgs["Sphere"] = sphereSubmesh;
	geo->DrawArgs["Cylinder"] = cylinderSubmesh;
	geo->DrawArgs["Quad"] = quadSubmesh;

	mGeometries[geo->Name] = std::move(geo);
}
void RenderScene::Tick(float DeltaTime)
{
	mRenderer->FlushFrameResourceQueue();
	Update(DeltaTime);
	mRenderer->Update(DeltaTime);
	if (mRenderer->mCamera != nullptr)
	{
		mRenderer->mCamera->UpdateViewMatrix();
	}
	mRenderer->Draw(DeltaTime);
}
void RenderScene::BuildMaterials()
{
	auto bricks0 = std::make_unique<Material>();
	bricks0->Name = "bricks0";
	bricks0->MatCBIndex = 0;
	bricks0->DiffuseSrvHeapIndex = 0;
	bricks0->NormalSrvHeapIndex = 1;
	bricks0->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	bricks0->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);
	bricks0->Roughness = 0.3f;

	auto tile0 = std::make_unique<Material>();
	tile0->Name = "tile0";
	tile0->MatCBIndex = 1;
	tile0->DiffuseSrvHeapIndex = 2;
	tile0->NormalSrvHeapIndex = 3;
	tile0->DiffuseAlbedo = XMFLOAT4(0.9f, 0.9f, 0.9f, 1.0f);
	tile0->FresnelR0 = XMFLOAT3(0.2f, 0.2f, 0.2f);
	tile0->Roughness = 0.1f;


	auto wood0 = std::make_unique<Material>();
	wood0->Name = "wood0";
	wood0->MatCBIndex = 2;
	wood0->DiffuseSrvHeapIndex = 4;
	wood0->NormalSrvHeapIndex = 5;
	wood0->DiffuseAlbedo = XMFLOAT4(0.9f, 0.9f, 0.9f, 1.0f);
	wood0->FresnelR0 = XMFLOAT3(0.98f, 0.97f, 0.95f);
	wood0->Roughness = 0.9f;


	auto paper0 = std::make_unique<Material>();
	paper0->Name = "paper0";
	paper0->MatCBIndex = 4;
	paper0->DiffuseSrvHeapIndex = 6;
	paper0->NormalSrvHeapIndex = 7;
	paper0->DiffuseAlbedo = XMFLOAT4(1.2f, 1.2f, 1.2f, 1.0f);
	paper0->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);
	paper0->Roughness = .99f;

	auto stone0 = std::make_unique<Material>();
	stone0->Name = "stone0";
	stone0->MatCBIndex = 5;
	stone0->DiffuseSrvHeapIndex = 8;
	stone0->NormalSrvHeapIndex = 9;
	stone0->DiffuseAlbedo = XMFLOAT4(0.9f, 0.9f, 0.9f, 1.0f);
	stone0->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);
	stone0->Roughness = .99f;
	
	auto mirror0 = std::make_unique<Material>();
	mirror0->Name = "mirror0";
	mirror0->MatCBIndex = 6;
	mirror0->DiffuseSrvHeapIndex = 10;
	mirror0->NormalSrvHeapIndex = 11;
	mirror0->DiffuseAlbedo = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mirror0->FresnelR0 = XMFLOAT3(0.98f, 0.97f, 0.95f);
	mirror0->Roughness = 0.1f;



	auto sky = std::make_unique<Material>();
	sky->Name = "sky";
	sky->MatCBIndex = 7;
	sky->DiffuseSrvHeapIndex = 12;
	sky->NormalSrvHeapIndex = 13;
	sky->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	sky->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);
	sky->Roughness = 1.0f;

	auto myMaterial = std::make_unique<Material>();
	myMaterial->Name = "myMaterial";
	myMaterial->MatCBIndex = 8;
	myMaterial->DiffuseSrvHeapIndex = 14;
	myMaterial->NormalSrvHeapIndex = 15;
	myMaterial->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	myMaterial->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);
	myMaterial->Roughness = 1.0f;

	mMaterials["bricks0"] = std::move(bricks0);
	mMaterials["tile0"] = std::move(tile0);
	mMaterials["mirror0"] = std::move(mirror0);
	mMaterials["sky"] = std::move(sky);
	mMaterials["myMaterial"] = std::move(myMaterial);
	mMaterials["wood0"] = std::move(wood0);
	mMaterials["paper0"] = std::move(paper0);
	mMaterials["stone0"] = std::move(stone0);
	
	mRenderer->mMatCBIndex = mMaterials.size();
}

std::unordered_map<std::string, std::shared_ptr<MeshGeometry>>& RenderScene::GetGeometries()
{
	return mGeometries;
}
std::unordered_map<std::string, std::unique_ptr<Material>>& RenderScene::GetMaterials()
{
	return mMaterials;
}
std::vector<std::unique_ptr<RenderItem>>& RenderScene::GetCurrRenderItem()
{

	return CurrRenderItem;

}

void RenderScene::Update(float DeltaTime)
{
	mRenderer->FlushCommandQueue();
	mLightRotationAngle += 0.1f * DeltaTime;

	XMMATRIX R = XMMatrixRotationY(mLightRotationAngle);
	for (int i = 0; i < 3; ++i)
	{
		XMVECTOR lightDir = XMLoadFloat3(&mBaseLightDirections[i]);
		lightDir = XMVector3TransformNormal(lightDir, R);
		XMStoreFloat3(&mRotatedLightDirections[i], lightDir);
	}
	std::vector<RenderItem*> NewItem;
	BoundingFrustum localSpaceFrustum;
	localSpaceFrustum.CreateFromMatrix(localSpaceFrustum, mainCamera->GetProj());
	localSpaceFrustum.Transform(localSpaceFrustum, XMMatrixInverse(nullptr,mainCamera->GetView()));
	for(RenderItem* item:mAllRitemLayer[(int)RenderLayer::Opaque])
	{
		BoundingBox box =  item->boundingBox;
		box.Transform(box,XMLoadFloat4x4(&item->World));
		if(localSpaceFrustum.Contains(box))
		{
			NewItem.push_back(item);
		}
	}
	for(auto& ModelInstance:mModelInstance)
	{
		if(ModelInstance.second.subRenderItem.size()<=0)continue;
		for(RenderItem* item:ModelInstance.second.subRenderItem)
		{
			BoundingBox box =  item->boundingBox;
			XMMATRIX world = XMLoadFloat4x4(&item->World);
			box.Transform(box,world);
			if(localSpaceFrustum.Contains(box))
			{
				NewItem.push_back(item);
			}
		}
	}
	mRitemLayer[(int)RenderLayer::Opaque] = std::move(NewItem);

	UpdateParticleCB();
	UpdateObjectCBs();
	UpdateShadowTransform();
	UpdateMainPassCB(DeltaTime);
	UpdateShadowPassCB();
}

void RenderScene::UpdateShadowTransform()
{
	// Only the first "main" light casts a shadow.
	XMVECTOR lightDir = XMLoadFloat3(&mRotatedLightDirections[0]);
	XMVECTOR lightPos = -22.0f*mSceneBounds.Radius*lightDir;
	XMVECTOR targetPos = mainCamera->GetPosition();
	XMVECTOR lightUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX lightView = XMMatrixLookAtLH(lightPos, targetPos, lightUp);

	XMStoreFloat3(&mLightPosW, lightPos);

	// Transform bounding sphere to light space.
	XMFLOAT3 sphereCenterLS;
	XMStoreFloat3(&sphereCenterLS, XMVector3TransformCoord(targetPos, lightView));

	// Ortho frustum in light space encloses scene.
	float l = sphereCenterLS.x - mSceneBounds.Radius;
	float b = sphereCenterLS.y - mSceneBounds.Radius;
	float n = sphereCenterLS.z - mSceneBounds.Radius;
	float r = sphereCenterLS.x + mSceneBounds.Radius;
	float t = sphereCenterLS.y + mSceneBounds.Radius;
	float f = sphereCenterLS.z + mSceneBounds.Radius;

	mLightNearZ = n;
	mLightFarZ = f;
	XMMATRIX lightProj = XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);

	// Transform NDC space [-1,+1]^2 to texture space [0,1]^2
	XMMATRIX T(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);

	XMMATRIX S = lightView*lightProj*T;
	XMStoreFloat4x4(&mLightView, lightView);
	XMStoreFloat4x4(&mLightProj, lightProj);
	XMStoreFloat4x4(&mShadowTransform, S);
}

void RenderScene::UpdateShadowPassCB()
{
	XMMATRIX view = XMLoadFloat4x4(&mLightView);
	XMMATRIX proj = XMLoadFloat4x4(&mLightProj);

	XMMATRIX viewProj = XMMatrixMultiply(view, proj);
	XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(view), view);
	XMMATRIX invProj = XMMatrixInverse(&XMMatrixDeterminant(proj), proj);
	XMMATRIX invViewProj = XMMatrixInverse(&XMMatrixDeterminant(viewProj), viewProj);

	//不应该在这里
	UINT w = mRenderer->ShadowPass->mShadowMap->Width();
	UINT h = mRenderer->ShadowPass->mShadowMap->Height();

	XMStoreFloat4x4(&mShadowPassCB.View, XMMatrixTranspose(view));
	XMStoreFloat4x4(&mShadowPassCB.InvView, XMMatrixTranspose(invView));
	XMStoreFloat4x4(&mShadowPassCB.Proj, XMMatrixTranspose(proj));
	XMStoreFloat4x4(&mShadowPassCB.InvProj, XMMatrixTranspose(invProj));
	XMStoreFloat4x4(&mShadowPassCB.ViewProj, XMMatrixTranspose(viewProj));
	XMStoreFloat4x4(&mShadowPassCB.InvViewProj, XMMatrixTranspose(invViewProj));
	mShadowPassCB.EyePosW = mLightPosW;
	mShadowPassCB.RenderTargetSize = XMFLOAT2((float)w, (float)h);
	mShadowPassCB.InvRenderTargetSize = XMFLOAT2(1.0f / w, 1.0f / h);
	mShadowPassCB.NearZ = mLightNearZ;
	mShadowPassCB.FarZ = mLightFarZ;

	auto currPassCB = GetCurrFrameResource()->PassCB.get();
	currPassCB->CopyData(1, mShadowPassCB);
}

void RenderScene::UpdateParticleCB()
{

	auto currInstanceCB = GetCurrFrameResource()->InstanceBuffer.get();
	auto currobjCB = GetCurrFrameResource()->ObjectCB.get();
	for (int i = 0 ; i<mRitemLayer[(int)RenderLayer::Particle].size();++i)
	{
		RenderItem* e = mRitemLayer[(int)RenderLayer::Particle][i];
		ParticleInstance& instance = *particleInstances[i].get();
		e->InstanceCount = instance.count;
		// Only update the cbuffer data if the constants have changed.  
		// This needs to be tracked per frame resource.
		if (e->NumFramesDirty > 0)
		{
			ObjectConstants objcb;
			XMMATRIX world = XMLoadFloat4x4(&(e->World));
			XMMATRIX texTransform = XMLoadFloat4x4(&(e->TexTransform));

			XMStoreFloat4x4(&objcb.World, XMMatrixTranspose(world));
			XMStoreFloat4x4(&objcb.TexTransform, XMMatrixTranspose(texTransform));
			objcb.MaterialIndex = e->Mat->MatCBIndex;
			
				
			currobjCB->CopyData(e->ObjCBIndex, objcb);
			for(int j = 0;j<instance.count;++j)
			{
				XMFLOAT4X4 xmfloat4_x4 = (*instance.worldTransform.get())[j];
				XMVECTOR particle_pos = {xmfloat4_x4.m[3][0],xmfloat4_x4.m[3][1],xmfloat4_x4.m[3][2]};
				XMVECTOR up = {0,1,0};

				XMFLOAT3 look3f = mainCamera->GetLook3f();
				float yaw = XMVector2AngleBetweenVectors({1,0},{-look3f.x,-look3f.z}).m128_f32[0];
				yaw = look3f.z>0?yaw:-yaw;
				auto res =  XMVector3AngleBetweenVectors(up,-mainCamera->GetLook());
				//XMMatrixLookAtRH({xmfloat4_x4.m[3][0],xmfloat4_x4.m[3][1],xmfloat4_x4.m[3][2]},mainCamera->GetPosition(),{0,1,});
				XMMATRIX world = XMMatrixRotationY(yaw)*XMMatrixRotationAxis(XMVector3Cross(-mainCamera->GetLook(),up),-res.m128_f32[0])*XMLoadFloat4x4(&(*instance.worldTransform.get())[j]);
				//XMMATRIX world = XMMatrixLookAtLH({xmfloat4_x4.m[3][0],xmfloat4_x4.m[3][1],xmfloat4_x4.m[3][2]},mainCamera->GetPosition(),{0,1,0});;
				XMMATRIX texTransform = XMLoadFloat4x4(&(*instance.TexTransform.get())[0]);
				
				InstanceData ObjInstanceData;
				XMStoreFloat4x4(&ObjInstanceData.World, XMMatrixTranspose(world));
				XMStoreFloat4x4(&ObjInstanceData.TexTransform, XMMatrixTranspose(texTransform));
				ObjInstanceData.MaterialIndex = e->Mat->MatCBIndex;
				
				currInstanceCB->CopyData(j, ObjInstanceData);
			}
			// Next FrameResource need to be updated too.
			e->NumFramesDirty--;
		}
	}
}


void RenderScene::UpdateMainPassCB(float deltaTime)
{
	totalTime+=deltaTime;
	if (mainCamera == nullptr)return;

	XMMATRIX view = mRenderer->mCamera->GetView();
	XMMATRIX proj = mRenderer->mCamera->GetProj();

	XMMATRIX viewProj = XMMatrixMultiply(view, proj);
	XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(view), view);
	XMMATRIX invProj = XMMatrixInverse(&XMMatrixDeterminant(proj), proj);
	XMMATRIX invViewProj = XMMatrixInverse(&XMMatrixDeterminant(viewProj), viewProj);

	// Transform NDC space [-1,+1]^2 to texture space [0,1]^2
	XMMATRIX T(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);

	XMMATRIX viewProjTex = XMMatrixMultiply(viewProj, T);
	XMMATRIX shadowTransform = XMLoadFloat4x4(&mShadowTransform);

	XMStoreFloat4x4(&mMainPassCB.View, XMMatrixTranspose(view));
	XMStoreFloat4x4(&mMainPassCB.InvView, XMMatrixTranspose(invView));
	XMStoreFloat4x4(&mMainPassCB.Proj, XMMatrixTranspose(proj));
	XMStoreFloat4x4(&mMainPassCB.InvProj, XMMatrixTranspose(invProj));
	XMStoreFloat4x4(&mMainPassCB.ViewProj, XMMatrixTranspose(viewProj));
	XMStoreFloat4x4(&mMainPassCB.InvViewProj, XMMatrixTranspose(invViewProj));
	XMStoreFloat4x4(&mMainPassCB.ViewProjTex, XMMatrixTranspose(viewProjTex));
	XMStoreFloat4x4(&mMainPassCB.ShadowTransform, XMMatrixTranspose(shadowTransform));
	mMainPassCB.EyePosW = mainCamera->GetPosition3f();
	mMainPassCB.RenderTargetSize = XMFLOAT2((float)mRenderer->mClientWidth, (float)mRenderer->mClientHeight);
	mMainPassCB.InvRenderTargetSize = XMFLOAT2(1.0f / mRenderer->mClientWidth, 1.0f / mRenderer->mClientHeight);
	mMainPassCB.NearZ = 1.0f;
	mMainPassCB.FarZ = 1000.0f;
	mMainPassCB.TotalTime = totalTime;
	mMainPassCB.DeltaTime =deltaTime;
	mMainPassCB.AmbientLight = { 0.25f, 0.25f, 0.35f, 1.0f };
	mMainPassCB.Lights[0].Direction = mRotatedLightDirections[0];
	mMainPassCB.Lights[0].Strength = { 0.9f, 0.9f, 0.7f };
	mMainPassCB.Lights[1].Direction = mRotatedLightDirections[1];
	mMainPassCB.Lights[1].Strength = { 0.4f, 0.4f, 0.4f };
	mMainPassCB.Lights[2].Direction = mRotatedLightDirections[2];
	mMainPassCB.Lights[2].Strength = { 0.2f, 0.2f, 0.2f };
	auto currPassCB = GetCurrFrameResource()->PassCB.get();
	currPassCB->CopyData(0, mMainPassCB);
}

void RenderScene::BuildGpuDefaultBuffer(Microsoft::WRL::ComPtr<ID3D12Resource>& BufferGPU, const void* initData,
                                        UINT64 byteSize, Microsoft::WRL::ComPtr<ID3D12Resource>& uploadBuffer)
{
	mRenderer->BuildGpuDefaultBuffer(BufferGPU, initData, byteSize, uploadBuffer);
}
