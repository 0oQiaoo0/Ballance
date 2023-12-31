#include "ModelLoader.h"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags


bool ModelLoader::LoadAssimpModel(const std::string& filename, std::vector<Vertex>& vertices,
                                  std::vector<USHORT>& indices, std::vector<Subset>& subsets, std::vector<M3dMaterial>& mats)
{
    // Create an instance of the Importer class
    Assimp::Importer importer;

    // And have it read the given file with some example postprocessing
    // Usually - if speed is not the most important aspect for you - you'll
    // probably to request more postprocessing than we do in this example.
    const aiScene* scene = importer.ReadFile( filename,
      aiProcess_CalcTangentSpace       |
      aiProcess_Triangulate            |
      aiProcess_JoinIdenticalVertices  |
      aiProcess_SortByPType);

    // If the import failed, report it
    if (nullptr == scene) {
        //DoTheErrorLogging( importer.GetErrorString());
        return false;
    }

    // Now we can access the file's contents.

    // We're done. Everything will be cleaned up by the importer destructor
    return true;
}
