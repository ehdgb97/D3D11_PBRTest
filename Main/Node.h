#pragma once
#include "Mesh.h"
#include "GameObject.h"
#include <string>
using namespace std;
using namespace DirectX::SimpleMath;
struct aiNode;
struct aiScene;

struct CB_Transform;
class GameObject;
class Node
{
private:
	GameObject* m_Owner;
	Node* m_pParent = nullptr;
	vector<Mesh*> m_meshes = {};
	vector<Node> m_Children = {};
	Matrix mTransformation = DirectX::XMMatrixIdentity();
	Matrix m_relativeMatrix = {};
	Matrix m_AnimationMatrix = {};
	bool m_haveAnime = false;
	
	string m_Name = {};
public:
	Node();
	Node(GameObject* Own);
	~Node();
	void SetOwner(GameObject* owner);
	void Create(aiNode* ai_Node);
	void Create(aiNode* ai_Node,Node* pParent);
	void Update();
	void Render(ID3D11DeviceContext* m_pDeviceContext, ID3D11BlendState* m_pAlphaBlendState, ID3D11Buffer* m_pMaterialCB, ID3D11Buffer* m_pTransformCB, ID3D11Buffer* m_pBoneTransformBuffer, CB_Transform* test);
	Matrix GetTransform();
	void SetAnimationMatrix(Matrix matrix)
	{
		m_haveAnime = true;
		m_AnimationMatrix = matrix;
	};
	const string GetName() const;
	Node* FindNode(std::string NodeName);
};
inline const string Node::GetName() const
{
	return m_Name;
}

inline Node* Node::FindNode(std::string NodeName)
{
	if (m_Name == NodeName)
		return this;
	for (auto& Child : m_Children)
	{
		Node* Founded = Child.FindNode(NodeName);
		if (Founded != nullptr)
			return Founded;
	}
	return nullptr;
}
