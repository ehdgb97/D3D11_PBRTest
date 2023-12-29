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
	//inline bool Init(GameObject* owner);	//이거 안할시 오류 터짐

};

//inline bool StaticMesh::Init(GameObject* owner)
//{
//	m_owner = owner;
//
//	if (owner)
//		return true;
//	else
//		return false;
//}
