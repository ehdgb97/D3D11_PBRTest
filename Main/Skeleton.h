#pragma once
#include "BoneReference.h"
#include "ConstantBuffers.h"
class Skeleton
{
	public:
		void Create(const aiScene* pScene);



		std::vector<BoneReference> m_BoneReferences;
		CB_MatrixPallete m_MatrixPalleteCB;

};

