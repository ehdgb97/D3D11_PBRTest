#pragma once
#include "Component.h"
class SkeletalMeshComponent :
    public Component
{
	SkeletalMeshComponent(Actor* Owner);
	//SkeletalMeshComponent(string _FilePath);
	~SkeletalMeshComponent();
	bool SetStaicMesh(string _FilePath);
	std::shared_ptr<FullSkeletonMesh> m_pFullSkeletonMesh;


	// Component을(를) 통해 상속됨
	void Update(float DeltaTime) override;

	// Component을(를) 통해 상속됨
	void Render() override;
};

