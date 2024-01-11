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


	// Component��(��) ���� ��ӵ�
	void Update(float DeltaTime) override;

	// Component��(��) ���� ��ӵ�
	void Render() override;
};

