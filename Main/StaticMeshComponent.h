#pragma once
#include "StaticMesh.h"
class StaticMesh;
class StaticMeshComponent
{
public:
	StaticMeshComponent();
	~StaticMeshComponent();
	std::shared_ptr<StaticMesh> m_pStaticMesh;
	bool SetStaicMesh(string _FilePath);

	bool Update();
};

