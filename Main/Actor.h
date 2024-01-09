#pragma once
class Component;
class Actor
{
public:
	Actor();
	~Actor();
	std::list<std::shared_ptr<Component>> m_OwnedComponents;	// 소유한 컴포넌트들
	virtual void Update(float _deltatime)=0;
	virtual void Render()=0;
};

