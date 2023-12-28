#pragma once
class Node;
using namespace DirectX::SimpleMath;
using namespace std;
class BoneReference
{
public:
	Node* m_pConnectNode=nullptr;
	Matrix m_offsetMatrix;
	string m_nodeName;
};