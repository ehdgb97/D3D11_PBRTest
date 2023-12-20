#pragma once
using namespace std;
using namespace DirectX::SimpleMath;

class BoneWeightVertex
{
public:
	Vector3 Position;
	Vector2 Texcoord;
	Vector3 Normal;
	Vector3 Tangent;
	int BlendIndeces[4] = {};
	float BlendWeights[4] = {};
	//����޴� ������ �ִ� 4���� ����!

	void AddBoneData(int boneIndex, float weight);
};
