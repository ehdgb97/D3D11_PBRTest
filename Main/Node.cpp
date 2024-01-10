#include "pch.h"
#include "Node.h"
#include "Helper.h"
#include <assimp/scene.h>
#include "Mesh.h"
#include "Actor.h"
#include "GameObject.h"



Node::Node()
{
}

Node::Node(GameObject* Own)
{
	m_Owner = Own;
}

Node::~Node()
{
}

void Node::SetOwner(GameObject* owner)
{
	m_Owner = owner;
}



void Node::Create(aiNode* ai_Node)
{
	string a= ai_Node->mName.C_Str();
	this->m_Name = ai_Node->mName.C_Str();
	this->m_pParent = nullptr;
	m_relativeMatrix = XMMatrixTranspose(DirectX::XMMATRIX(ai_Node->mTransformation[0]));

	//m_Owner->GetNodes().push_back(this);

	m_meshes.resize(ai_Node->mNumMeshes);
	for (size_t i = 0; i < ai_Node->mNumMeshes; ++i)
		m_meshes[i]= m_Owner->GetMeshes()[ai_Node->mMeshes[i]];

	m_Children.resize(ai_Node->mNumChildren);
	for (size_t i = 0; i < ai_Node->mNumChildren; ++i)
	{
		m_Children[i].SetOwner(m_Owner);
		m_Children[i].Create(ai_Node->mChildren[i],this);
	}

}

void Node::Create(aiNode* ai_Node, Node* pParent)
{
	this->m_Name = ai_Node->mName.C_Str();
	this->m_pParent = pParent;
	this->m_Owner = m_Owner;
	m_relativeMatrix = XMMatrixTranspose(DirectX::XMMATRIX(ai_Node->mTransformation[0]));
	//m_Owner->GetNodes().push_back(this);

	if (m_Owner)
	{
		m_meshes.resize(ai_Node->mNumMeshes);
		for (size_t i = 0; i < ai_Node->mNumMeshes; ++i)
		{
			m_meshes[i] = m_Owner->GetMeshes()[ai_Node->mMeshes[i]];
		}
	}
	

	m_Children.resize(ai_Node->mNumChildren);
	for (size_t i = 0; i < ai_Node->mNumChildren; ++i)
	{
		m_Children[i].SetOwner(m_Owner);
		m_Children[i].Create(ai_Node->mChildren[i],this);
	}


}

void Node::Update()
{
	if (m_Owner != nullptr)
	{

	}
	else
	{
		Matrix matrix = DirectX::XMMatrixIdentity();
		if (!m_Owner->GetUseAni())
		{
			if (m_pParent)
				matrix = m_pParent->mTransformation;

			else
				matrix = m_Owner->GetWorld();

			mTransformation = m_relativeMatrix * matrix;
		}
		else
		{
			if (m_haveAnime)
			{
				if (m_pParent)
					matrix = m_pParent->mTransformation;
				else
					matrix = m_Owner->GetWorld();
				mTransformation = m_AnimationMatrix * matrix;
			}
			else
			{
				if (m_pParent)
					matrix = m_pParent->mTransformation;
				else
					matrix = m_Owner->GetWorld();

				mTransformation = m_relativeMatrix * matrix;
			}

		}
	}
	


	for (auto& node : m_Children)
		node.Update();
}

void Node::Render(ID3D11DeviceContext* m_pDeviceContext, ID3D11BlendState* m_pAlphaBlendState, ID3D11Buffer* m_pMaterialCB, ID3D11Buffer* m_pTransformCB, ID3D11Buffer* m_pBoneTransformBuffer, CB_Transform* test)
{
	test->mWorld = DirectX::XMMatrixTranspose(mTransformation);
	for (auto mesh: m_meshes)
	{
		mesh->Render(m_pDeviceContext, m_pAlphaBlendState, m_pMaterialCB, m_pTransformCB, m_pBoneTransformBuffer, test);
	}

	for (auto& node : m_Children)
		node.Render(m_pDeviceContext, m_pAlphaBlendState, m_pMaterialCB, m_pTransformCB, m_pBoneTransformBuffer,test);

}

Matrix Node::GetTransform()
{
	return mTransformation;
}

