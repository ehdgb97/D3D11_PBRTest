#include "pch.h"
#include "StaticMeshComponent.h"
#include"ResourceManager.h"
StaticMeshComponent::StaticMeshComponent()
{
}

StaticMeshComponent::~StaticMeshComponent()
{
}

bool StaticMeshComponent::SetStaicMesh(string _FilePath)
{
	m_pStaticMesh =ResourceManager::Instance->Search_StaticMesh(_FilePath);
	return true;
}


bool StaticMeshComponent::Update()
{
	return true;
}
