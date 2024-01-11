#pragma once
class Actor;
class Component
{
public:
	Component(Actor* Owner):m_pOwner(Owner){};
	 virtual ~Component()=default;
	 Actor* m_pOwner = nullptr;
	 std::string m_Name;			
	 void SetName(std::string val) { m_Name = val; }


	 Actor* GetOwner() { return m_pOwner; }
	 virtual void Update(float DeltaTime);
	 virtual void Render();

};

