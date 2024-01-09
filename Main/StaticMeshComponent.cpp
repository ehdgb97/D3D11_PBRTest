#include "pch.h"
#include "StaticMesh.h"
#include "StaticMeshComponent.h"
#include"D3DRenderManager.h"
#include"ResourceManager.h"
StaticMeshComponent::StaticMeshComponent()
{
}

StaticMeshComponent::StaticMeshComponent(string _FilePath)
{
	SetStaicMesh(_FilePath);
}

StaticMeshComponent::~StaticMeshComponent()
{
}

bool StaticMeshComponent::SetStaicMesh(string _FilePath)
{
	m_pStaticMesh =ResourceManager::Instance->Search_StaticMesh(_FilePath);
	return true;
}


void StaticMeshComponent::Update(float DeltaTime)
{
	m_pStaticMesh->m_rootNode.Update();

}

void StaticMeshComponent::Render()
{
	for (auto part:m_pStaticMesh->m_pStaticMeshPart)
	{
		Matrix transform=	DirectX::XMMatrixTranspose(m_pStaticMesh->m_rootNode.FindNode(part.m_Name)->GetTransform());
		D3DRenderManager::Instance->m_TransformCB.mWorld = transform;




	}
	
}
