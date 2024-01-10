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
    using namespace DirectX;
    XMFLOAT3 meshRotation = {};
    meshRotation.x = XMConvertToRadians(m_angle.x);
    meshRotation.y = XMConvertToRadians(m_angle.y);
    meshRotation.z = XMConvertToRadians(m_angle.z);
    Matrix scaleMatrix = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
    Matrix rotationMatrix = XMMatrixRotationX(meshRotation.x) * XMMatrixRotationY(meshRotation.y) * XMMatrixRotationZ(meshRotation.z);
    Matrix translationMatrix = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
    mLocalTransformMatrix = scaleMatrix * rotationMatrix * translationMatrix;

    m_angle.x = static_cast<float>(fmod(m_angle.x, 360));
    m_angle.y = static_cast<float>(fmod(m_angle.y, 360));
    m_angle.z = static_cast<float>(fmod(m_angle.z, 360));
    m_scale = { m_scale.x,m_scale.x,m_scale.x };
    //if (m_useAnime)
    //{
    //    this->m_pAnimation[this->AnimationIndex]->Update();
    //}



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
