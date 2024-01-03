#pragma once
#include "StaticMeshPart.h"
#include "Material.h"
#include "Node.h"

class Material;

class StaticMesh
{
public:
	StaticMesh();
	~StaticMesh();
	std::vector<StaticMeshPart> m_pStaticMeshPart;
	std::vector<Material> m_Materials;
	bool Create(std::string _FilePath);
	Node m_rootNode;

};

