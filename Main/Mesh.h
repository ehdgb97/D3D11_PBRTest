#pragma once
#include "ConstantBuffers.h"

using namespace DirectX::SimpleMath;
using namespace std;

// ���� ����.

class BoneWeightVertex;
struct Vertex;
class BoneReference;
struct CB_MatrixPallete;
struct aiMesh;
struct CB_Transform;
class Material;
class GameObject;

class Mesh
{
public:
	Mesh(GameObject* owner);
	~Mesh();
public:
	void CreateVertexBuffer( Vertex* vertices, UINT vertexCount);
	void CreateBoneWeightVertexBuffer( BoneWeightVertex* vertices, UINT vertexCount);
	void CreateIndexBuffer( UINT* indies, UINT indexCount);
	void Create( aiMesh* mesh);
	void Create_Bone( aiMesh* mesh);
	void Render(ID3D11DeviceContext* m_pDeviceContext, ID3D11BlendState* m_pAlphaBlendState, ID3D11Buffer* m_pMaterialCB, ID3D11Buffer* m_pTransformCB, ID3D11Buffer* m_pBoneTransformBuffer, CB_Transform* test);
	void UpdateMatrixPallete(ID3D11DeviceContext* deviceContext, ID3D11Buffer* m_pBoneTransformBuffer);
	GameObject*  m_owner;
	ID3D11Buffer* m_pVertexBuffer = nullptr;
	ID3D11Buffer* m_pIndexBuffer=nullptr;
	Material* m_pMaterial = nullptr;
	std::string m_name;
	vector<BoneWeightVertex> m_BoneWeightVertexs;
	vector<BoneReference> m_BoneReferences;
	CB_MatrixPallete m_matrixPalleteCB;
	UINT m_VertexCount = 0;
	UINT m_VertexBufferStride = 0;						// ���ؽ� �ϳ��� ũ��.
	UINT m_VertexBufferOffset = 0;						// ���ؽ� ������ ������.
	UINT m_IndexCount = 0;				// �ε��� ����.
	UINT m_MaterialIndex = 0;			// ���׸��� �ε���.
};

