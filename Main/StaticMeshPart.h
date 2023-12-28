#pragma once
using namespace DirectX::SimpleMath;
using namespace std;

struct Vertex;

class StaticMeshPart
{
public:
	StaticMeshPart();
	~StaticMeshPart();
public:
	std::string m_Name;
	void CreateIndexBuffer(UINT* indies, UINT indexCount);
	void CreateVertexBuffer(Vertex* vertices, UINT vertexCount);

	void Create(aiMesh* mesh);
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;

	UINT m_VertexCount = 0;
	UINT m_VertexBufferStride = 0;						// ���ؽ� �ϳ��� ũ��.
	UINT m_VertexBufferOffset = 0;						// ���ؽ� ������ ������.
	UINT m_IndexCount = 0;				// �ε��� ����.

};

