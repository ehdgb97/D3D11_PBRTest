#pragma once
#include "ConstantBuffers.h"

class Component;
class Actor
{

	Vector3 m_position = {};
	Vector3 m_scale = { 2.f,2.f,2.f };
	Vector3 m_angle = {};
	Matrix mLocalTransformMatrix;
	Matrix     mWorld;		// 월드좌표계 공간으로 변환을 위한 행렬.
	bool m_useAnime = false;


public:
	unsigned int AnimationIndex = 0;

	Actor();
	~Actor();
	std::list<std::shared_ptr<Component>> m_OwnedComponents;	// 소유한 컴포넌트들
	virtual void Update(float _deltatime);
	virtual void Render();

	void SetWorld(Matrix world);
	void SetPos(Vector3 Pos);
	void SetScale(Vector3 Scale);
	void SetAngle(Vector3 Angle);
	void SetUseAni(bool Use);

	Matrix GetWorld();
	Vector3 GetPos();
	Vector3 GetScale();
	Vector3 GetAngle();
	bool GetUseAni();
	//const vector<Animation*>& GetAnimations() const;






	template<typename Comp>
	std::shared_ptr<Comp> AddComponent()
	{
		std::shared_ptr<Comp> newComponent = std::make_shared<Comp>(this);
		m_OwnedComponents.push_back(newComponent);

		return newComponent;
	}
};

inline void Actor::SetUseAni(bool Use)
{
	m_useAnime = Use;

}

inline bool Actor::GetUseAni()
{
	return m_useAnime;
}

inline void Actor::SetWorld(Matrix world)
{
	mWorld = mLocalTransformMatrix * world;
}

inline void Actor::SetPos(Vector3 Pos)
{
	m_position = Pos;
}
inline void Actor::SetScale(Vector3 Scale)
{
	m_scale = Scale;
}
inline void Actor::SetAngle(Vector3 Angle)
{
	m_angle = Angle;
}
inline Vector3 Actor::GetPos()
{
	return m_position;
}
inline Vector3 Actor::GetScale()
{
	return m_scale;
}
inline Vector3 Actor::GetAngle()
{
	return m_angle;
}

inline Matrix Actor::GetWorld()
{
	return mWorld;
}
//inline vector<Node*>& Actor::GetNodes()
//{
//	return m_Nodes;
//}
//inline const vector<Animation*>& Actor::GetAnimations() const
//{
//	return m_pAnimation;
//}
