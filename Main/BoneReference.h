#pragma once
class Mesh;
class Node;
using namespace DirectX::SimpleMath;
using namespace std;
class BoneReference
{
public:
	//Mesh* m_pOwner;
	Node* m_pConnectNode;
	Matrix m_offsetMatrix;
	string m_nodeName;
	//int m_index;

};