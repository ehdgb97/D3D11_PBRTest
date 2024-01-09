#pragma once
#include "VertexType.h"
#include "BoneReference.h"
#include "ConstantBuffers.h"
class Node;
class SkeletalMeshPart
{
public:
	SkeletalMeshPart();
	~SkeletalMeshPart();
public:
	void CreateBoneWeightVertexBuffer(BoneWeightVertex* vertices, UINT vertexCount);
	void CreateIndexBuffer(UINT* indies, UINT indexCount);
	void Create_Bone(aiMesh* mesh, Node* RootNode);
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;

	vector<BoneWeightVertex> m_BoneWeightVertexs;
	vector<BoneReference> m_BoneReferences;
	CB_MatrixPallete m_matrixPalleteCB;
	UINT m_VertexCount = 0;
	UINT m_VertexBufferStride = 0;						// ���ؽ� �ϳ��� ũ��.
	UINT m_VertexBufferOffset = 0;						// ���ؽ� ������ ������.
	UINT m_IndexCount = 0;				// �ε��� ����.
	UINT m_MaterialIndex = 0;			// ���׸��� �ε���.
	std::string m_Name;
	std::string m_NodeName;
	void UpdateMatrixPallete(ID3D11DeviceContext* deviceContext, ID3D11Buffer* m_pBoneTransformBuffer);
	void SetNodeName(std::string name) { m_NodeName = name; }

};

