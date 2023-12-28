#pragma once
class GameObject;
class StaticMeshPart;
class Node;
class Material;
class StaticMesh
{
public:
	StaticMesh();
	~StaticMesh();
	GameObject* m_owner = nullptr;
	std::vector<StaticMeshPart> m_pStaticMeshPart;
	std::vector<Material> m_Materials;
	bool Create(std::string _FilePath);
	Node m_rootNode;
	//inline bool Init(GameObject* owner);	//�̰� ���ҽ� ���� ����

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
