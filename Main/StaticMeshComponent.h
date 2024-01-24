#pragma once
class Actor;
#include"Component.h"

class StaticMesh;
class StaticMeshComponent :public Component
{
public:


	

	StaticMeshComponent(Actor* owner);
	//StaticMeshComponent(std::string _FilePath);
	~StaticMeshComponent();
	bool SetStaicMesh(std::string _FilePath);
	std::shared_ptr<StaticMesh> m_pStaticMesh;
	DirectX::BoundingBox m_BoundingBox;
	DirectX::SimpleMath::Vector3 m_BoundingBoxCenterOffset = DirectX::SimpleMath::Vector3::Zero;

	// Component��(��) ���� ��ӵ�
	void Update(float DeltaTime) override;

	// Component��(��) ���� ��ӵ�
	void Render() override;

	// Component��(��) ���� ��ӵ�
	void DebugRender() override;

};

