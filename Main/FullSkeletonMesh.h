#pragma once
#include "SkeletalMeshPart.h"

class FullSkeletonMesh
{
public:
	std::string m_FBXName;
	std::string m_FilePath;
	std::vector<SkeletalMeshPart*> m_pSkeletalMeshPart;

	bool Create(std::string _FilePath);
	inline void SetFBXName(std::string _FBXName);
	inline void SetFilePath(std::string _FilePath);
	inline std::string GetFBXName();
	inline std::string GetFilePath();
	void AddSkeletalMeshPart(SkeletalMeshPart* _pSkeletalMeshPart);

};



inline void FullSkeletonMesh::SetFBXName(std::string _FBXName)
{
	m_FBXName = _FBXName;
}
inline void FullSkeletonMesh::SetFilePath(std::string _FilePath)
{
	m_FilePath = _FilePath;
}

inline std::string FullSkeletonMesh::GetFBXName()
{
	return m_FBXName;
}
inline std::string FullSkeletonMesh::GetFilePath()
{
	return m_FilePath;
}


inline void FullSkeletonMesh::AddSkeletalMeshPart(SkeletalMeshPart* _pSkeletalMeshPart)
{
	m_pSkeletalMeshPart.push_back(_pSkeletalMeshPart);
}

