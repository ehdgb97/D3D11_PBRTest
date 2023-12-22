#pragma once
#include "Mesh.h"
#include "FullSkeletonMesh.h"
#include"Animation.h"
#include<string>

class ResourceManager
{
	std::map<std::string, std::weak_ptr<FullSkeletonMesh>> m_resouce_FullSkeletonMesh;
	std::map<std::string, std::weak_ptr<Animation>> m_resouce_Animation;

	public:
	std::shared_ptr<FullSkeletonMesh> Search_FullSkeletonMesh(std::string _Filename);
	std::shared_ptr<Animation> Search_Animation(std::string _Filename);

};
