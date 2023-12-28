#include "pch.h"
#include "StaticMeshPart.h"
#include "Helper.h"
#include "D3DRenderManager.h"
#include "VertexType.h"


StaticMeshPart::StaticMeshPart()
{
}

StaticMeshPart::~StaticMeshPart()
{
}

void StaticMeshPart::CreateIndexBuffer(UINT* indices, UINT indexCount)
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

void StaticMeshPart::CreateVertexBuffer(Vertex* vertices, UINT vertexCount)
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

void StaticMeshPart::Create(aiMesh* mesh)
{
	// 버텍스 정보 생성
	m_Name = mesh->mName.C_Str();
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