#include "pch.h"
#include "ResourceManager.h"
#include "Helper.h"
#include "Mesh.h"
#include "FullSkeletonMesh.h"
#include"StaticMesh.h"
#include"Animation.h"
#include "TexturePack.h"
ResourceManager* ResourceManager::Instance = nullptr;

ResourceManager::ResourceManager()
{
	assert(Instance == nullptr);
	Instance = this;
}

ResourceManager::~ResourceManager()
{
}

std::shared_ptr<FullSkeletonMesh> ResourceManager::Search_FullSkeletonMesh(std::string _Filename)
{
	auto it = m_resource_FullSkeletonMesh.find(_Filename);
	if (it != m_resource_FullSkeletonMesh.end())
	{
		std::shared_ptr<FullSkeletonMesh> resourcePtr = it->second.lock();
		if (resourcePtr)  //UseCount가 1이상이면 리턴.
			return resourcePtr;
		else  //UseCount가 0이라면 제거.
			m_resource_FullSkeletonMesh.erase(it);
	}
	std::shared_ptr<FullSkeletonMesh> pFullSkeletonMesh = std::make_shared<FullSkeletonMesh>();
	pFullSkeletonMesh->Create(_Filename);
	m_resource_FullSkeletonMesh[_Filename] = pFullSkeletonMesh;
	return pFullSkeletonMesh;
}

std::shared_ptr<Animation> ResourceManager::Search_Animation(std::string _Filename)
{
	return std::shared_ptr<Animation>();
}

std::shared_ptr<TextureImage> ResourceManager::Search_TextureImage(std::wstring _Filename)
{
	auto it = m_TextureImageMap.find(_Filename);
	if (it != m_TextureImageMap.end())
	{
		std::shared_ptr<TextureImage> resourcePtr = it->second.lock();
		if (resourcePtr)  //UseCount가 1이상이면 리턴.
			return resourcePtr;
		else  //UseCount가 0이라면 제거.
			m_TextureImageMap.erase(it);
	}


	std::shared_ptr<TextureImage> pTextureImage = std::make_shared<TextureImage>();
	pTextureImage->Create(_Filename);
	m_TextureImageMap[_Filename] = pTextureImage;
	return pTextureImage;
}

std::shared_ptr<StaticMesh> ResourceManager::Search_StaticMesh(std::string _Filename)
{
	auto it = m_resource_StaticMesh.find(_Filename);
	if (it != m_resource_StaticMesh.end())
	{
		std::shared_ptr<StaticMesh> resourcePtr = it->second.lock();
		if (resourcePtr)  //UseCount가 1이상이면 리턴.
			return resourcePtr;
		else  //UseCount가 0이라면 제거.
			m_resource_StaticMesh.erase(it);
	}
	std::shared_ptr<StaticMesh> pStaticMesh = std::make_shared<StaticMesh>();
	pStaticMesh->Create(_Filename);
	m_resource_StaticMesh[_Filename] = pStaticMesh;
	return pStaticMesh;
}

