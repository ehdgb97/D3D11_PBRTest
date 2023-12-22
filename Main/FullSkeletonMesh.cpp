#include "pch.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/config.h>
#include <assimp/cimport.h>
#include "FullSkeletonMesh.h"
#include "Helper.h"
#include "D3DRenderManager.h"

bool FullSkeletonMesh::Create(std::string _FilePath)
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
        m_Materials[i] = new Material();
        m_Materials[i]->Create(D3DRenderManager::m_pDevice, scene->mMaterials[i]);
    }

    m_Meshes.resize(scene->mNumMeshes);


    for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
    {
        m_Meshes[i] = new Mesh(this);
        if (scene->mMeshes[i]->HasBones())
        {
            m_Meshes[i]->Create_Bone(scene->mMeshes[i]);
        }
        else
        {
            m_Meshes[i]->Create(scene->mMeshes[i]);
        }
    }
    m_rootNode = make_shared<Node>(this);
    m_rootNode->Create(scene->mRootNode);

    //m_pAnimation.resize(scene->mNumAnimations);
    //for (size_t i = 0; i < scene->mNumAnimations; ++i)
    //{
    //    m_pAnimation[i] = new Animation(this, scene->mAnimations[i]->mDuration);
    //    for (size_t j = 0; j < scene->mAnimations[i]->mNumChannels; j++)
    //    {
    //        m_pAnimation[i]->Create(scene->mAnimations[i]->mChannels[j]);
    //    }
    //}

    importer.FreeScene();
    return true;


}
