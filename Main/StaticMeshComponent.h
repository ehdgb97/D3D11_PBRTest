#pragma once
#include"Component.h"
class StaticMesh;

class StaticMeshComponent :public Component
{
public:
	StaticMeshComponent();
	StaticMeshComponent(string _FilePath);
	~StaticMeshComponent();
	bool SetStaicMesh(string _FilePath);
	std::shared_ptr<StaticMesh> m_pStaticMesh;


	// Component을(를) 통해 상속됨
	void Update(float DeltaTime) override;

	// Component을(를) 통해 상속됨
	void Render() override;
};

