#include "pch.h"
#include "StaticMesh.h"
#include "StaticMeshComponent.h"
#include"D3DRenderManager.h"
#include"ResourceManager.h"

StaticMeshComponent::StaticMeshComponent()
{
}

StaticMeshComponent::StaticMeshComponent(string _FilePath)
{
	SetStaicMesh(_FilePath);
}

StaticMeshComponent::~StaticMeshComponent()
{
}

bool StaticMeshComponent::SetStaicMesh(string _FilePath)
{
	m_pStaticMesh =ResourceManager::Instance->Search_StaticMesh(_FilePath);
	return true;
}


void StaticMeshComponent::Update(float DeltaTime)
{
	m_pStaticMesh->m_rootNode.Update();

}

void StaticMeshComponent::Render()
{
	for (auto part:m_pStaticMesh->m_pStaticMeshPart)
	{
		Matrix transform = DirectX::XMMatrixTranspose(m_pStaticMesh->m_rootNode.FindNode(part.m_Name)->GetTransform());
		D3DRenderManager::Instance->m_TransformCB.mWorld = transform;
		//m_pStaticMesh->m_Materials[part.m_MaterialIndex].m_MaterialCB.Ambient = m_owner->GetMaterialCB().Ambient;
		//m_pStaticMesh->m_Materials[part.m_MaterialIndex].m_MaterialCB.Diffuse = m_owner->GetMaterialCB().Diffuse;
		//m_pStaticMesh->m_Materials[part.m_MaterialIndex].m_MaterialCB.Emissive = m_owner->GetMaterialCB().Emissive;
		//m_pStaticMesh->m_Materials[part.m_MaterialIndex].m_MaterialCB.Specular = m_owner->GetMaterialCB().Specular;
		//m_pStaticMesh->m_Materials[part.m_MaterialIndex].m_MaterialCB.SpecularPower = m_owner->GetMaterialCB().SpecularPower;
		m_pStaticMesh->m_Materials[part.m_MaterialIndex].Render();
		D3DRenderManager::Instance->m_pDeviceContext->UpdateSubresource(D3DRenderManager::Instance->m_pTransformCB, 0, nullptr, &D3DRenderManager::Instance->m_TransformCB, 0, 0);
		D3DRenderManager::Instance->m_pDeviceContext->IASetIndexBuffer(part.m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		D3DRenderManager::Instance->m_pDeviceContext->IASetVertexBuffers(0, 1, &part.m_pVertexBuffer, &part.m_VertexBufferStride, &part.m_VertexBufferOffset);
		D3DRenderManager::Instance->m_pDeviceContext->DrawIndexed(part.m_IndexCount, 0, 0);
	}
}
