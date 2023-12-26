#include "pch.h"
#include"D3DRenderManager.h"
#include "GameObject.h"

#include <assimp/postprocess.h>

#include "Helper.h"

struct CB_Transform;

bool GameObject::SetFBX(string Filename)
{
    // 8. FBX Loading

    Assimp::Importer importer;
    unsigned int importFlags = aiProcess_Triangulate |
        aiProcess_GenNormals |
        aiProcess_GenUVCoords |
        aiProcess_CalcTangentSpace |
        aiProcess_LimitBoneWeights|
        aiProcess_ConvertToLeftHanded;
    string path="../Resource/";
    string realpath = path + Filename;
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
        m_Materials[i]->Create(scene->mMaterials[i]);
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

    m_pAnimation.resize(scene->mNumAnimations);
    for (size_t i = 0; i < scene->mNumAnimations; ++i)
    {
        m_pAnimation[i] = new Animation(this, scene->mAnimations[i]->mDuration);
        for (size_t j = 0; j < scene->mAnimations[i]->mNumChannels; j++)
        {
            m_pAnimation[i]->Create(scene->mAnimations[i]->mChannels[j]);
        }
    }

    importer.FreeScene();
    return true;
}

bool GameObject::Update(ID3D11DeviceContext* deviceContext, ID3D11Buffer* m_pBoneTransformBuffer)
{
    using namespace DirectX;
	XMFLOAT3 meshRotation = {};
    meshRotation.x = XMConvertToRadians(m_angle.x);
    meshRotation.y = XMConvertToRadians(m_angle.y);
    meshRotation.z = XMConvertToRadians(m_angle.z);
    Matrix scaleMatrix = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
    Matrix rotationMatrix = XMMatrixRotationX(meshRotation.x) * XMMatrixRotationY(meshRotation.y) * XMMatrixRotationZ(meshRotation.z);
    Matrix translationMatrix = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
    mLocalTransformMatrix = scaleMatrix * rotationMatrix * translationMatrix;

    m_angle.x = static_cast<float>(fmod(m_angle.x, 360));
    m_angle.y = static_cast<float>(fmod(m_angle.y, 360));
    m_angle.z = static_cast<float>(fmod(m_angle.z, 360));
	m_scale = { m_scale.x,m_scale.x,m_scale.x };
    if (m_useAnime)
    {
        this->m_pAnimation[this->AnimationIndex]->Update();
    }

    for (auto node : m_Nodes)
        node->Update();

    for (auto Meshes : m_Meshes)
    {
        Meshes->UpdateMatrixPallete(deviceContext, m_pBoneTransformBuffer);
    }
    return true;
}

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
  
	for (auto ptr : m_pAnimation)
	{
		delete ptr;
	}
    for (auto ptr : m_Materials)
    {
        delete ptr;
    }
    for (auto ptr : m_Meshes)
    {
        delete ptr;
    }



}

bool GameObject::Render(ID3D11DeviceContext* m_pDeviceContext,ID3D11BlendState* m_pAlphaBlendState, ID3D11Buffer* m_pMaterialCB, ID3D11Buffer* m_pTransformCB, ID3D11Buffer* m_pBoneTransformBuffer, CB_Transform* test)
{
    if (m_pAnimation.size()> this->AnimationIndex)
    {
		this->m_pAnimation[this->AnimationIndex]->Render();
    } 
        m_rootNode->Render(m_pDeviceContext, m_pAlphaBlendState, m_pMaterialCB, m_pTransformCB, m_pBoneTransformBuffer,test);

    return true;
}


void GameObject::SetMaterialCB(CB_Material materialcb)
{
    this->m_MaterialCB = materialcb;
}

Matrix GameObject::GetWorld()
{
    return mWorld;
}

CB_Material GameObject::GetMaterialCB()
{
    return m_MaterialCB;
}
