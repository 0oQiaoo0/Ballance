#include "ModelLoader.h"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <climits>
#include <cfloat>

#include "macro.h"
#include <cfloat>

#include "../GameEngine.h"
ModelLoader* ModelLoader::_instance = nullptr;



ModelLoader* ModelLoader::GetInstance()
{
    if(_instance==nullptr)
    {
        _instance = new ModelLoader;
    }
    return _instance;   
}

const ModelLoader::ModelData& ModelLoader::GetModelData(std::string& filename)
{
    if(AllModelData.count(filename)<=0)
    {
        LOG_WARN("no Model Name"+filename);
        globalContext.mGameEngine->mScene->renderScene->BuildModelMeshInfo(filename);
    }
    const ModelData& data_info = AllModelData[filename];

    return data_info;
}

bool ModelLoader::LoadAssimpModel(const std::string& filename)
{
    if(AllModelData.count(filename)>0)
    {
        LOG_WARN("Model "+filename+" has load");
        return true;
    }
    Assimp::Importer importer;

    // And have it read the given file with some example postprocessing
    // Usually - if speed is not the most important aspect for you - you'll
    // probably to request more postprocessing than we do in this example.
    const aiScene* scene = importer.ReadFile( filename,
      aiProcess_CalcTangentSpace       |
      aiProcess_Triangulate            |
      aiProcess_JoinIdenticalVertices  |
      aiProcess_FlipUVs |
      aiProcess_SortByPType);

    // If the import failed, report it
    if (nullptr == scene) {
        //DoTheErrorLogging( importer.GetErrorString());
        return false;
    }
    _vertices.clear();
    _indices.clear();
    _subsets.clear();
    _mats.clear();
    _mats_loaded.clear();
    aiMaterialIndex.clear();
    subsetMaterialIndex.clear();
    processNode(scene->mRootNode,scene);
    ModelData  model_data;
    model_data.path = filename;
    model_data.indiceCount = _indices.size();
    model_data.indiceStart = global_indices.size();
    model_data.verticeCount = _vertices.size();
    model_data.verticeStart = global_vertices.size();
    model_data.subsetStart = global_subsets.size();
    model_data.subsetCount = _subsets.size();
    model_data.materialStart = global_mats.size();
    //todo make no copy
    global_indices.insert(global_indices.end(),
    std::make_move_iterator(_indices.begin()),
    std::make_move_iterator(_indices.end()));
    global_vertices.insert(global_vertices.end(),
        std::make_move_iterator(_vertices.begin()),
        std::make_move_iterator(_vertices.end()));

    AllModelData[filename] = model_data;

    global_subsets.insert(global_subsets.end(),
        std::make_move_iterator(_subsets.begin()),
        std::make_move_iterator(_subsets.end()));
    global_mats.insert(global_mats.end(),
        std::make_move_iterator(_mats.begin()),
        std::make_move_iterator(_mats.end()));
    
    // We're done. Everything will be cleaned up by the importer destructor
    return true;
}

bool ModelLoader::LoadAssimpModel(const std::string& filename, std::vector<Vertex>& vertices,
                                  std::vector<UINT32>& indices, std::vector<Subset>& subsets, std::vector<M3dMaterial>& mats)
{
    // Create an instance of the Importer class
    Assimp::Importer importer;

    // And have it read the given file with some example postprocessing
    // Usually - if speed is not the most important aspect for you - you'll
    // probably to request more postprocessing than we do in this example.
    const aiScene* scene = importer.ReadFile( filename,
      aiProcess_CalcTangentSpace       |
    
      aiProcess_JoinIdenticalVertices  |
      aiProcess_FlipUVs |
      aiProcess_SortByPType);

    // If the import failed, report it
    if (nullptr == scene) {
        //DoTheErrorLogging( importer.GetErrorString());
        return false;
    }
    subsetIndex = 0;
    vertices.clear();
    subsets.clear();
    indices.clear();
    mats.clear();
    _vertices.clear();
    _indices.clear();
    _subsets.clear();
    _mats.clear();
    _mats_loaded.clear();
    
    processNode(scene->mRootNode,scene);
    
    // Now we can access the file's contents.
    vertices = _vertices;
    subsets = _subsets;
    indices = _indices;
    global_indices.insert(global_indices.end(),
        std::make_move_iterator(_indices.begin()),
        std::make_move_iterator(_indices.end()));
    global_vertices.insert(global_vertices.end(),
        std::make_move_iterator(_vertices.begin()),
        std::make_move_iterator(_vertices.end()));
    global_subsets.insert(global_subsets.end(),
        std::make_move_iterator(_subsets.begin()),
        std::make_move_iterator(_subsets.end()));
    //todo load material
    mats=_mats;
    // We're done. Everything will be cleaned up by the importer destructor
    return true;
}

void ModelLoader::processNode(aiNode* node, const aiScene* scene)
{
    if(node==nullptr||scene==nullptr)return;
    for (UINT i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        _subsets.push_back(this->processMesh(mesh, scene));
    }

    for (UINT i = 0; i < node->mNumChildren; i++) {
        this->processNode(node->mChildren[i], scene);
    }
}
//场景中还有很多贴图以及常量没有加载，待完善
ModelLoader::M3dMaterial ModelLoader::loadMaterialTextures(
    aiMaterial * mat, aiTextureType type,
    std::string typeName, const aiScene * scene,
    std::string& OwnMesh) {
   ModelLoader::M3dMaterial texture;
    texture.Name = OwnMesh;
    for (UINT i = 0; i < mat->GetTextureCount(aiTextureType::aiTextureType_DIFFUSE); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        texture.DiffuseMapName = str.C_Str();

        break;
    }
    for (UINT i = 0; i < mat->GetTextureCount(aiTextureType::aiTextureType_NORMALS); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        texture.NormalMapName = str.C_Str();
        break;
    }
    return texture;
}
M3DLoader::Subset ModelLoader::processMesh(aiMesh* mesh, const aiScene* scene)
{
    float minZ = FLT_MAX;
    float minY = FLT_MAX;
    float minX = FLT_MAX;
    float maxZ =FLT_MIN;
    float maxY = FLT_MIN;
    float maxX = FLT_MIN;

    std::string MeshName =  mesh->mName.C_Str();
    MeshName=MeshName+"_"+std::to_string(subsetIndex);
    UINT VertexStart = _vertices.size();
    UINT FaceStart = _indices.size()/3;
    for (UINT i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex{};

        vertex.Pos.x = mesh->mVertices[i].x;
        vertex.Pos.y = mesh->mVertices[i].y;
        vertex.Pos.z = mesh->mVertices[i].z;
        minZ = min(vertex.Pos.z,minZ);
        minY = min(vertex.Pos.y,minY);
        minX = min(vertex.Pos.x,minX);
        maxZ = max(vertex.Pos.z,maxZ);
        maxY = max(vertex.Pos.y,maxY);
        maxX = max(vertex.Pos.x,maxX);
        if(mesh->mNormals!=nullptr)
        {
            vertex.Normal.x = mesh->mNormals[i].x;
            vertex.Normal.y = mesh->mNormals[i].y;
            vertex.Normal.z = mesh->mNormals[i].z;
        }
        if (mesh->mTextureCoords[0]) {
            vertex.TexC.x = (float)mesh->mTextureCoords[0][i].x;
            vertex.TexC.y = (float)mesh->mTextureCoords[0][i].y;
        }
        if(mesh->mTangents!=nullptr)
        {
            vertex.TangentU.x = mesh->mTangents[i].x;
            vertex.TangentU.y = mesh->mTangents[i].y;
            vertex.TangentU.z = mesh->mTangents[i].z;
            vertex.TangentU.w = 1;//todo: what is this?
        }

        _vertices.push_back(vertex);
    }

    for (UINT i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];

        for (UINT j = 0; j < face.mNumIndices; j++)
            _indices.push_back(face.mIndices[j]);
    }
    unsigned int materialIndex = -1;
    if (mesh->mMaterialIndex > 0) {
        ModelLoader::M3dMaterial MaterialMaps;
      
        if(aiMaterialIndex.count(mesh->mMaterialIndex)>0)
        {
            materialIndex = aiMaterialIndex[mesh->mMaterialIndex];
            MaterialMaps=_mats[materialIndex];
        }
        else
        {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            MaterialMaps= this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", scene,MeshName);
            aiMaterialIndex[mesh->mMaterialIndex] = _mats.size();
            materialIndex =  _mats.size();
            _mats.push_back(MaterialMaps);
        }
       
    }
    DirectX::XMFLOAT3 center((minX+maxX)/2,(minY+maxY)/2,(minZ+maxZ)/2);
    DirectX::XMFLOAT3 extent(maxX-center.x,maxY-center.y,maxZ-center.z);
    Subset subset{subsetIndex++,VertexStart,
        mesh->mNumVertices,FaceStart,
        mesh->mNumFaces,materialIndex,
        MeshName,DirectX::BoundingBox(center,extent)};

    return subset;
}
