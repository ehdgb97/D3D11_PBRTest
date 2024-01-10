#include "pch.h"
#include "Actor.h"
#include"Component.h"
#include"StaticMeshComponent.h"
Actor::Actor()
{
	auto newComponent = AddComponent<StaticMeshComponent>();
	newComponent->SetStaicMesh("cerberus.fbx");
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
