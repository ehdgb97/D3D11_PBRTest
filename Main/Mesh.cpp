#include "pch.h"
#include "Mesh.h"
#include "GameObject.h"
#include "Helper.h"
#include "ConstantBuffers.h"
#include "D3DRenderManager.h"
#include <map>

using namespace DirectX;

Mesh::Mesh(GameObject* owner)
{
	m_owner = owner;
}

Mesh::~Mesh()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
}

void Mesh::CreateVertexBuffer( Vertex* vertices, UINT vertexCount)
{
	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = sizeof(Vertex) * vertexCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA vbData = {};
	vbData.pSysMem = vertices;
	HR_T(D3DRenderManager::m_pDevice->CreateBuffer(&bd, &vbData, &m_pVertexBuffer));

	// 버텍스 버퍼 정보
	m_VertexCount = vertexCount;
	m_VertexBufferStride = sizeof(Vertex);
	m_VertexBufferOffset = 0;
}

void Mesh::CreateBoneWeightVertexBuffer( BoneWeightVertex* vertices, UINT vertexCount)
{
	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = sizeof(BoneWeightVertex) * vertexCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA vbData = {};
	vbData.pSysMem = vertices;
	HR_T(D3DRenderManager::m_pDevice->CreateBuffer(&bd, &vbData, &m_pVertexBuffer));

	// 버텍스 버퍼 정보
	m_VertexCount = vertexCount;
	m_VertexBufferStride = sizeof(BoneWeightVertex);
	m_VertexBufferOffset = 0;
}

void Mesh::CreateIndexBuffer(UINT* indices, UINT indexCount)
{
	// 인덱스 개수 저장.
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


void Mesh::Create(aiMesh* mesh)
{
	m_MaterialIndex = mesh->mMaterialIndex;
	m_pMaterial = m_owner->GetMaterials()[m_MaterialIndex];
	// 버텍스 정보 생성
	unique_ptr<Vertex[]> vertices(new Vertex[mesh->mNumVertices]);
	for (UINT i = 0; i < mesh->mNumVertices; ++i)
	{
		vertices[i].Position = Vector3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertices[i].Normal = Vector3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		vertices[i].Texcoord = Vector2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		vertices[i].Tangent = Vector3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
	}
	CreateVertexBuffer(vertices.get(), mesh->mNumVertices);

	// 인덱스 정보 생성
	unique_ptr<UINT[]> indices(new UINT[mesh->mNumFaces * 3]);

	for (UINT i = 0; i < mesh->mNumFaces; ++i)
	{
		indices[i * 3 + 0] = mesh->mFaces[i].mIndices[0];
		indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
		indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
	}


	CreateIndexBuffer(indices.get(), mesh->mNumFaces * 3);


}

void Mesh::Create_Bone(aiMesh* mesh)
{

		m_MaterialIndex = mesh->mMaterialIndex;
		m_pMaterial = m_owner->GetMaterials()[m_MaterialIndex];
		// 버텍스 정보 생성
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


		// 인덱스 정보 생성
		unique_ptr<UINT[]> indices(new UINT[mesh->mNumFaces * 3]);

		for (UINT i = 0; i < mesh->mNumFaces; ++i)
		{
			indices[i * 3 + 0] = mesh->mFaces[i].mIndices[0];
			indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
			indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
		}

		CreateIndexBuffer(indices.get(), mesh->mNumFaces * 3);


		UINT meshBoneCount = mesh->mNumBones; // 메쉬와 연결된 본개수 
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
void Mesh::Render(ID3D11DeviceContext* m_pDeviceContext, ID3D11BlendState* m_pAlphaBlendState, ID3D11Buffer* m_pMaterialCB, ID3D11Buffer* m_pTransformCB, ID3D11Buffer* m_pBoneTransformBuffer, CB_Transform* test)
{
	m_pMaterial->m_materialCB.Ambient = m_owner->GetMaterialCB().Ambient;
	m_pMaterial->m_materialCB.Diffuse = m_owner->GetMaterialCB().Diffuse;
	m_pMaterial->m_materialCB.Emissive = m_owner->GetMaterialCB().Emissive;
	m_pMaterial->m_materialCB.Specular= m_owner->GetMaterialCB().Specular;
	m_pMaterial->m_materialCB.SpecularPower= m_owner->GetMaterialCB().SpecularPower;

	m_pMaterial->Render(m_pDeviceContext, m_pAlphaBlendState, m_pMaterialCB);
	m_pDeviceContext->UpdateSubresource(m_pTransformCB, 0, nullptr, test, 0, 0);
	m_pDeviceContext->UpdateSubresource(m_pBoneTransformBuffer, 0, nullptr, &m_matrixPalleteCB, 0, 0);
	m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_VertexBufferStride, &m_VertexBufferOffset);
	m_pDeviceContext->DrawIndexed(m_IndexCount, 0, 0);
}

void Mesh::UpdateMatrixPallete(ID3D11DeviceContext* deviceContext, ID3D11Buffer* m_pBoneTransformBuffer)
{
	for (auto a : m_owner->GetNodes())
		for (auto& b : m_BoneReferences)
			if (a->GetName() == b.m_nodeName)
				b.m_pConnectNode = a;

	// 본 트랜스폼 계산
	assert(m_BoneReferences.size() < 128);
	for (UINT i = 0; i < m_BoneReferences.size(); i++)
	{
		Matrix BoneNodeWorldMatrix = m_BoneReferences[i].m_pConnectNode->GetTransform();
		m_matrixPalleteCB.Array[i] = (m_BoneReferences[i].m_offsetMatrix * BoneNodeWorldMatrix); 
		m_matrixPalleteCB.Array[i] = m_matrixPalleteCB.Array[i].Transpose();
	}

}