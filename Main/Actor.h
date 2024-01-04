#pragma once
class Component;
class Actor
{
public:
	Actor();
	~Actor();
	std::list<std::shared_ptr<Component>> m_OwnedComponents;	// ������ ������Ʈ��
	void Update(float _deltatime);
	void Render();
};

