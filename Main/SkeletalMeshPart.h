#pragma once
#include "BoneWeightVertex.h"
#include "BoneReference.h"
#include "ConstantBuffers.h"
#include "Material.h"
#include "Skeleton.h"

class SkeletalMeshPart
{
public:
	SkeletalMeshPart();
	~SkeletalMeshPart();
public:
	void CreateBoneWeightVertexBuffer(BoneWeightVertex* vertices, UINT vertexCount);
	void CreateIndexBuffer(UINT* indies, UINT indexCount);
	void Create_Bone(aiMesh* mesh);
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;

	vector<BoneWeightVertex> m_BoneWeightVertexs;
	vector<BoneReference> m_BoneReferences;
	Skeleton m_Skeleton;
	CB_MatrixPallete m_matrixPalleteCB;
	UINT m_VertexCount = 0;
	UINT m_VertexBufferStride = 0;						// ���ؽ� �ϳ��� ũ��.
	UINT m_VertexBufferOffset = 0;						// ���ؽ� ������ ������.
	UINT m_IndexCount = 0;				// �ε��� ����.
	UINT m_MaterialIndex = 0;			// ���׸��� �ε���.
	std::string m_name;
};

