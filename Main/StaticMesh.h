#pragma once
#include "StaticMeshPart.h"
#include "Material.h"
#include "Node.h"

class Material;
using namespace DirectX::SimpleMath;
class StaticMesh
{
public:
	StaticMesh();
	~StaticMesh();
	std::vector<StaticMeshPart> m_pStaticMeshPart;
	std::vector<Material> m_Materials;
	bool Create(std::string _FilePath);
	bool Create_meshes(aiNode* node, const aiScene* scene);
	Node m_rootNode;
	Vector3 m_AABBmin;
	Vector3 m_AABBmax;
	Vector3 m_BoundingBoxMin;	// 회전을 고려한 느슨한AABB
	Vector3 m_BoundingBoxMax;	// 회전을 고려한 느슨한AABB

};

