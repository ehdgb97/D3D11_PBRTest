#pragma once
#include "SkeletalMeshPart.h"
#include"Node.h"
class FullSkeletonMesh
{
public:
	GameObject* m_owner=nullptr;
	FullSkeletonMesh();
	~FullSkeletonMesh();
	//std::string m_FBXName;
	//std::string m_FilePath;
	std::vector<SkeletalMeshPart> m_pSkeletalMeshPart;
	std::vector<Material> m_Materials;
	Node m_rootNode;
	bool Create(std::string _FilePath);


};

