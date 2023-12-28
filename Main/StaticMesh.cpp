#include "pch.h"
#include "Node.h"
#include "StaticMesh.h"
#include "Material.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/config.h>
#include <assimp/cimport.h>
#include "StaticMeshPart.h"

#include "Helper.h"
#include "D3DRenderManager.h"
#include "ConstantBuffers.h"
StaticMesh::StaticMesh()
{
}

StaticMesh::~StaticMesh()
{
}

bool StaticMesh::Create(std::string _FilePath)
{
    // 8. FBX Loading

    Assimp::Importer importer;
    unsigned int importFlags = aiProcess_Triangulate |
        aiProcess_GenNormals |
        aiProcess_GenUVCoords |
        aiProcess_CalcTangentSpace |
        aiProcess_LimitBoneWeights |
        aiProcess_ConvertToLeftHanded;
    string path = "../Resource/";
    string realpath = path + _FilePath;
    importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0);    // $assimp_fbx$ 노드 생성안함
    const aiScene* scene = importer.ReadFile(realpath, importFlags);
    if (!scene) {
        LOG_ERRORA("Error loading FBX file: %s", importer.GetErrorString());
        return false;
    }
    m_Materials.resize(scene->mNumMaterials);
    for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
    {
        m_Materials[i].Create(scene->mMaterials[i]);
    }

    m_pStaticMeshPart.resize(scene->mNumMeshes);


    for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
    {
            m_pStaticMeshPart[i].Create(scene->mMeshes[i]);
    }
    m_rootNode.Create(scene->mRootNode);

    importer.FreeScene();
    return true;
}

