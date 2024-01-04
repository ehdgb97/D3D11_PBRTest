#include "pch.h"
#include "Actor.h"
#include"Component.h"
Actor::Actor()
{
}

Actor::~Actor()
{
}

void Actor::Update(float _deltatime)
{
	for (auto& component :m_OwnedComponents)
	{
		component->Update(_deltatime);
	}
}

void Actor::Render()
{
	for (auto& component : m_OwnedComponents)
	{
		component->Render();
	}
}
