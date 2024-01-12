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

	// Component��(��) ���� ��ӵ�
	void Update(float DeltaTime) override;

	// Component��(��) ���� ��ӵ�
	void Render() override;
};

