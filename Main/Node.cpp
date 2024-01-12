#include "pch.h"
#include "Node.h"
#include "Helper.h"
#include <assimp/scene.h>
#include "Actor.h"
 
#include"Component.h"


Node::Node()
{
}

//Node::Node(Actor* Own)
//{
//	m_Owner = Own;
//}

Node::~Node()
{
}

void Node::SetOwner(Component* owner)
{
	m_Owner = owner;
	for (auto& child: m_Children)
	{
		child.SetOwner(m_Owner);
	}

}



void Node::Create(aiNode* ai_Node)
{
	string a= ai_Node->mName.C_Str();
	this->m_Name = ai_Node->mName.C_Str();
	this->m_pParent = nullptr;
	m_relativeMatrix = XMMatrixTranspose(DirectX::XMMATRIX(ai_Node->mTransformation[0]));

	//m_Owner->GetNodes().push_back(this);

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



	m_Children.resize(ai_Node->mNumChildren);
	for (size_t i = 0; i < ai_Node->mNumChildren; ++i)
	{
		m_Children[i].SetOwner(m_Owner);
		m_Children[i].Create(ai_Node->mChildren[i],this);
	}


}

void Node::Update()
{
	assert(m_Owner != nullptr);
	Matrix matrix = DirectX::XMMatrixIdentity();
	//if (!m_Owner->GetUseAni())
	//{
		if (m_pParent)
			matrix = m_pParent->mTransformation;

		else
			matrix = m_Owner->GetWorld();

		mTransformation = m_relativeMatrix * matrix;
	//}
	//else
	//{
	//	if (m_haveAnime)
	//	{
	//		if (m_pParent)
	//			matrix = m_pParent->mTransformation;
	//		else
	//			matrix = m_Owner->GetWorld();
	//		mTransformation = m_AnimationMatrix * matrix;
	//	}
	//	else
	//	{
	//		if (m_pParent)
	//			matrix = m_pParent->mTransformation;
	//		else
	//			matrix = m_Owner->GetWorld();

	//		mTransformation = m_relativeMatrix * matrix;
	//	}

	//}



	for (auto& node : m_Children)
		node.Update();
}

void Node::Render(ID3D11DeviceContext* m_pDeviceContext, ID3D11BlendState* m_pAlphaBlendState, ID3D11Buffer* m_pMaterialCB, ID3D11Buffer* m_pTransformCB, ID3D11Buffer* m_pBoneTransformBuffer, CB_Transform* test)
{

}

Matrix Node::GetTransform()
{
	return mTransformation;
}

