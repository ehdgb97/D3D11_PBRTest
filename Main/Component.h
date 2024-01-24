#pragma once
//#include "ConstantBuffers.h"
using namespace DirectX::SimpleMath;

class Actor;
class Component
{
public:
	Vector3 m_position = {};
	Vector3 m_scale = { 1.f,1.f,1.f };
	Vector3 m_angle = {};
	Matrix mLocalTransformMatrix;
	Matrix     mWorld;		// 월드좌표계 공간으로 변환을 위한 행렬.
	Matrix GetWorld();



	Component(Actor* Owner):m_pOwner(Owner){};
	 virtual ~Component()=default;
	 Actor* m_pOwner = nullptr;
	 std::string m_Name;			

	 void SetName(std::string val) { m_Name = val; }


	 Actor* GetOwner() { return m_pOwner; }
	 virtual void Update(float DeltaTime);
	 virtual void Render();
	 virtual void DebugRender();

};

inline Matrix Component::GetWorld()
{
	return mWorld;
}