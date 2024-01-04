#pragma once
class Actor;
class Component
{
public:
	Component() {};
	 virtual ~Component()=default;
	 Actor* m_pOwner = nullptr;
	 std::string m_Name;			
	 void SetName(std::string val) { m_Name = val; }


	 Actor* GetOwner() { return m_pOwner; }
	 virtual void Update(float DeltaTime) = 0;
	 virtual void Render() = 0;

};

