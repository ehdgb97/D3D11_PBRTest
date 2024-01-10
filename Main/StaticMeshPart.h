#pragma once
#include "VertexType.h"

using namespace DirectX::SimpleMath;
using namespace std;
class Node;

class StaticMeshPart
{
public:
	StaticMeshPart();
	~StaticMeshPart();
public:
	std::string m_Name;
	std::string pNodeName;
	void CreateIndexBuffer(UINT* indies, UINT indexCount);
	void CreateVertexBuffer(Vertex* vertices, UINT vertexCount);
	unsigned int m_MaterialIndex = 0; 

	void Create(aiMesh* mesh);
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	UINT m_VertexCount = 0;
	UINT m_VertexBufferStride = 0;						// 버텍스 하나의 크기.
	UINT m_VertexBufferOffset = 0;						// 버텍스 버퍼의 오프셋.
	UINT m_IndexCount = 0;				// 인덱스 개수.
	void SetNodeName(std::string name) { pNodeName = name; }
	std::string GetNodeNaem() const {	return pNodeName;}
};

