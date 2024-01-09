#include "pch.h"
#include "StaticMesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/config.h>
#include <assimp/cimport.h>

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
    m_rootNode.Create(scene->mRootNode);

    m_Materials.resize(scene->mNumMaterials);


    for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
    {
        m_Materials[i].Create(scene->mMaterials[i]);
    }

    m_pStaticMeshPart.resize(scene->mNumMeshes);

    Create_meshes(scene->mRootNode, scene);

    /*for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
    {
		m_pStaticMeshPart[i].Create(scene->mMeshes[i]);
    }*/

    importer.FreeScene();
    return true;
}

bool StaticMesh::Create_meshes(aiNode* node, const aiScene* scene)
{
    // 현재 노드에 연결된 메시의 이름 출력
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		unsigned int meshIndex = node->mMeshes[i];
		aiMesh* mesh = scene->mMeshes[meshIndex];
		mesh->mName.C_Str();
		m_pStaticMeshPart[i].Create(mesh);
		m_pStaticMeshPart[i].SetNodeName(node->mName.C_Str());

	}

	// 자식 노드에 대해 재귀적으로 호출
	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		Create_meshes(node->mChildren[i], scene);
	}


	return true;
}

