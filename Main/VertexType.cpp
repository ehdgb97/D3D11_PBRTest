#include "pch.h"
#include "VertexType.h"

void BoneWeightVertex::AddBoneData(int boneIndex, float weight)
{
	assert(BlendWeights[0] == 0.0f || BlendWeights[1] == 0.0f || BlendWeights[2] == 0.0f || BlendWeights[3] == 0.0f);
	for (size_t i = 0; i < 4; i++)
	{
		if (BlendWeights[i] == 0.0f)
		{
			BlendIndeces[i] = boneIndex;
			BlendWeights[i] = weight;
			return;
		}
	}

}
