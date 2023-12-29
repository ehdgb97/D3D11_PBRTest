#pragma once
#include "Material.h"
#include "Node.h"
#include "SkeletalMeshPart.h"
class FullSkeletonMesh
{
public:
	FullSkeletonMesh();
	~FullSkeletonMesh();
	std::vector<SkeletalMeshPart> m_pSkeletalMeshPart;
	std::vector<Material> m_Materials;
	Node m_rootNode;
	bool Create(std::string _FilePath);


};

