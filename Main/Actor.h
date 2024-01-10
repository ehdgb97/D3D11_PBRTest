#pragma once
class Component;
class Actor
{
public:
	Actor();
	~Actor();
	std::list<std::shared_ptr<Component>> m_OwnedComponents;	// 소유한 컴포넌트들
	virtual void Update(float _deltatime);
	virtual void Render();
	template<typename Comp>
	std::shared_ptr<Comp> AddComponent()
	{
		std::shared_ptr<Comp> newComponent = std::make_shared<Comp>();
		m_OwnedComponents.push_back(newComponent);

		return newComponent;
	}
};

