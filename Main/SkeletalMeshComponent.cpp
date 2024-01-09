#include "pch.h"
#include"FullSkeletonMesh.h"
#include "SkeletalMeshComponent.h"
#include "ResourceManager.h"
SkeletalMeshComponent::SkeletalMeshComponent()
{
}

SkeletalMeshComponent::SkeletalMeshComponent(string _FilePath)
{
	SetStaicMesh(_FilePath);
}

SkeletalMeshComponent::~SkeletalMeshComponent()
{
}

bool SkeletalMeshComponent::SetStaicMesh(string _FilePath)
{
	m_pFullSkeletonMesh = ResourceManager::Instance->Search_FullSkeletonMesh(_FilePath);
	return true;
}

void SkeletalMeshComponent::Update(float DeltaTime)
{




}

void SkeletalMeshComponent::Render()
{
}
