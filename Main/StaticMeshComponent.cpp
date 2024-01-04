#include "pch.h"
#include "StaticMesh.h"
#include "StaticMeshComponent.h"

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
}

void StaticMeshComponent::Render()
{
}
