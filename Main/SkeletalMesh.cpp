#include "pch.h"
#include "SkeletalMesh.h"
#include "D3DRenderManager.h"
#include "pch.h"
#include "Mesh.h"
#include "GameObject.h"
#include "Helper.h"
#include "ConstantBuffers.h"
#include <map>

using namespace DirectX;

SkeletalMesh::SkeletalMesh()
{
}

SkeletalMesh::~SkeletalMesh()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
}

void SkeletalMesh::CreateBoneWeightVertexBuffer(BoneWeightVertex* vertices, UINT vertexCount)
{
	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = sizeof(BoneWeightVertex) * vertexCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA vbData = {};
	vbData.pSysMem = vertices;
	HR_T(D3DRenderManager::m_pDevice->CreateBuffer(&bd, &vbData, &m_pVertexBuffer));

	// ���ؽ� ���� ����
	m_VertexCount = vertexCount;
	m_VertexBufferStride = sizeof(BoneWeightVertex);
	m_VertexBufferOffset = 0;
}

void SkeletalMesh::CreateIndexBuffer(UINT* indices, UINT indexCount)
{
	// �ε��� ���� ����.
	m_IndexCount = indexCount;

	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = sizeof(UINT) * indexCount;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA ibData = {};
	ibData.pSysMem = indices;
	HR_T(D3DRenderManager::m_pDevice->CreateBuffer(&bd, &ibData, &m_pIndexBuffer));
}



void SkeletalMesh::Create_Bone(aiMesh* mesh)
{
	m_name = mesh->mName.C_Str();

	// ���ؽ� ���� ����
	unique_ptr<BoneWeightVertex[]> vertices(new BoneWeightVertex[mesh->mNumVertices]);
	for (UINT i = 0; i < mesh->mNumVertices; ++i)
	{
		BoneWeightVertex Bonevertex;
		Bonevertex.Position = Vector3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		Bonevertex.Normal = Vector3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		Bonevertex.Texcoord = Vector2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		Bonevertex.Tangent = Vector3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
		m_BoneWeightVertexs.push_back(Bonevertex);
		Bonevertex.Normal.Normalize();
		Bonevertex.Tangent.Normalize();
	}


	// �ε��� ���� ����
	unique_ptr<UINT[]> indices(new UINT[mesh->mNumFaces * 3]);

	for (UINT i = 0; i < mesh->mNumFaces; ++i)
	{
		indices[i * 3 + 0] = mesh->mFaces[i].mIndices[0];
		indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
		indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
	}

	CreateIndexBuffer(indices.get(), mesh->mNumFaces * 3);


	UINT meshBoneCount = mesh->mNumBones; // �޽��� ����� ������ 
	m_BoneReferences.resize(meshBoneCount); //

	UINT boneIndexCounter = 0;
	std::map<std::string, int> boneMapping;
	for (UINT i = 0; i < meshBoneCount; ++i)
	{
		aiBone* bone = mesh->mBones[i];
		std::string boneName = bone->mName.C_Str();
		UINT boneIndex = 0;
		if (boneMapping.find(boneName) == boneMapping.end())
		{
			boneIndex = boneIndexCounter;
			boneIndexCounter++;
			m_BoneReferences[boneIndex].m_nodeName = boneName;
			m_BoneReferences[boneIndex].m_offsetMatrix = Matrix(&bone->mOffsetMatrix.a1).Transpose();
			boneMapping[boneName] = boneIndex;
		}
		else
			boneIndex = boneMapping[boneName];

		// Map bone name to bone index
		for (size_t j = 0; j < bone->mNumWeights; ++j)
		{
			UINT vertexID = bone->mWeights[j].mVertexId;
			float weight = bone->mWeights[j].mWeight;
			m_BoneWeightVertexs[vertexID].AddBoneData(boneIndex, weight);
		}
	}

	CreateBoneWeightVertexBuffer(&m_BoneWeightVertexs[0], (UINT)m_BoneWeightVertexs.size());

}
void SkeletalMesh::Render(ID3D11DeviceContext* m_pDeviceContext, ID3D11BlendState* m_pAlphaBlendState, ID3D11Buffer* m_pMaterialCB, ID3D11Buffer* m_pTransformCB, ID3D11Buffer* m_pBoneTransformBuffer, CB_Transform* test)
{
	//m_pMaterial->m_materialCB.Ambient = m_owner->GetMaterialCB().Ambient;
	//m_pMaterial->m_materialCB.Diffuse = m_owner->GetMaterialCB().Diffuse;
	//m_pMaterial->m_materialCB.Emissive = m_owner->GetMaterialCB().Emissive;
	//m_pMaterial->m_materialCB.Specular = m_owner->GetMaterialCB().Specular;
	//m_pMaterial->m_materialCB.SpecularPower = m_owner->GetMaterialCB().SpecularPower;

	//m_pMaterial->Render(m_pDeviceContext, m_pAlphaBlendState, m_pMaterialCB);
	//m_pDeviceContext->UpdateSubresource(m_pTransformCB, 0, nullptr, test, 0, 0);
	//m_pDeviceContext->UpdateSubresource(m_pBoneTransformBuffer, 0, nullptr, &m_matrixPalleteCB, 0, 0);
	//m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_VertexBufferStride, &m_VertexBufferOffset);
	//m_pDeviceContext->DrawIndexed(m_IndexCount, 0, 0);
}

