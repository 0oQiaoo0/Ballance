#pragma once
#include <intsafe.h>
#include <map>
#include <vector>
#include <assimp/scene.h>

#include "../Render/dx12/FrameResource.h"
#include "../Render/dx12/LoadM3d.h"
class ModelLoader:public M3DLoader
{
public:
    struct ModelData
    {
        std::string path;
        long long indiceCount = -1;
        long long indiceStart = -1;
        long long subsetCount = -1;
        long long subsetStart = -1;
        long long verticeCount =-1;
        long long verticeStart =-1;
        long long materialStart = -1;
    };

    static  ModelLoader* GetInstance();
    //modelData 只是存储了所有数据的偏移量
    const ModelData& GetModelData(std::string& filename);
    
    bool LoadAssimpModel(const std::string& filename);
    

    bool LoadAssimpModel(const std::string& filename, 
    std::vector<Vertex>& vertices,
    std::vector<UINT32>& indices,
    std::vector<Subset>& subsets,
    std::vector<M3dMaterial>& mats);
    
    //全局数据
    std::vector<Vertex> global_vertices;
    std::vector<UINT32> global_indices;
    std::vector<Subset> global_subsets;
    std::vector<M3dMaterial> global_mats;
    std::unordered_map<std::string,ModelData> AllModelData;

protected:
    void processNode(aiNode* node, const aiScene* scene);
    M3dMaterial loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName,
                                                  const aiScene* scene, std::string& OwnMesh);
    Subset processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Vertex> _vertices;
    std::vector<UINT32> _indices;
    std::vector<Subset> _subsets;
    std::vector<M3dMaterial> _mats;
    
    std::vector<M3dMaterial> _mats_loaded;//废弃
    std::map<unsigned int,unsigned int> aiMaterialIndex;
    std::map<unsigned int,unsigned int> subsetMaterialIndex;

    UINT subsetIndex = 0;
private:
    ModelLoader(){};
    static ModelLoader* _instance;
};
