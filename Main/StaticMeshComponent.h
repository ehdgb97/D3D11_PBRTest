#pragma once
class Actor;
#include"Component.h"

class StaticMesh;
class StaticMeshComponent :public Component
{
public:
	Matrix ToltalTransform;
	Matrix mLocalTransformMatrix;

	Vector3 m_position = {};
	Matrix     mWorld;		// 월드좌표계 공간으로 변환을 위한 행렬.
	Vector3 m_scale = { .3f,.3f,.3f };
	Vector3 m_angle = {};

	StaticMeshComponent(Actor* owner);
	//StaticMeshComponent(std::string _FilePath);
	~StaticMeshComponent();
	bool SetStaicMesh(std::string _FilePath);
	std::shared_ptr<StaticMesh> m_pStaticMesh;

	// Component을(를) 통해 상속됨
	void Update(float DeltaTime) override;

	// Component을(를) 통해 상속됨
	void Render() override;
};

