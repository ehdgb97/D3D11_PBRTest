#include "pch.h"
#include "ResourceManager.h"
#include "D3DRenderManager.h"
#include "Helper.h"
#include <assimp/scene.h>
#include <assimp/postprocess.h>

std::shared_ptr<FullSkeletonMesh> ResourceManager::Search_FullSkeletonMesh(std::string _Filename)
{
	auto it = m_resouce_FullSkeletonMesh.find(_Filename);
	if (it != m_resouce_FullSkeletonMesh.end())
	{
		std::shared_ptr<FullSkeletonMesh> resourcePtr = it->second.lock();
		if (resourcePtr)  //UseCount가 1이상이면 리턴.
			return resourcePtr;
		else  //UseCount가 0이라면 제거.
			m_resouce_FullSkeletonMesh.erase(it);
	}
	std::shared_ptr<FullSkeletonMesh> pFullSkeletonMesh = std::make_shared<FullSkeletonMesh>();
	pFullSkeletonMesh->Create(_Filename);
	m_resouce_FullSkeletonMesh[_Filename] = pFullSkeletonMesh;
	return pFullSkeletonMesh;
}

std::shared_ptr<Animation> ResourceManager::Search_Animation(std::string _Filename)
{
	return std::shared_ptr<Animation>();
}

