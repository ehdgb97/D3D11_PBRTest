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
	//inline void SetFBXName(std::string _FBXName);
	//inline void SetFilePath(std::string _FilePath);
	//inline std::string GetFBXName();
	//inline std::string GetFilePath();
	inline bool Init(GameObject* owner);	//이거 안할시 오류 터짐

};


//inline void FullSkeletonMesh::SetFBXName(std::string _FBXName)
//{
//	m_FBXName = _FBXName;
//}
//inline void FullSkeletonMesh::SetFilePath(std::string _FilePath)
//{
//	m_FilePath = _FilePath;
//}
//
//inline std::string FullSkeletonMesh::GetFBXName()
//{
//	return m_FBXName;
//}
//inline std::string FullSkeletonMesh::GetFilePath()
//{
//	return m_FilePath;
//}

inline bool FullSkeletonMesh::Init(GameObject* owner)
{
	m_owner = owner;

	if (owner)
		return true;
	else
		return false;
}



//inline void FullSkeletonMesh::AddSkeletalMeshPart(SkeletalMeshPart _pSkeletalMeshPart)
//{
//	m_pSkeletalMeshPart.push_back(_pSkeletalMeshPart);
//}
//
