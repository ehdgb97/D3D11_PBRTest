#pragma once
using namespace std;
using namespace DirectX::SimpleMath;
struct aiNode;
struct aiScene;
class Mesh;
struct CB_Transform;
class Actor;
class Component;
class Node
{
private:
	Component* m_Owner = nullptr;

	Node* m_pParent = nullptr;
	vector<Node> m_Children = {};
	Matrix mTransformation = DirectX::XMMatrixIdentity();
	Matrix m_relativeMatrix = {};
	Matrix m_AnimationMatrix = {};
	bool m_haveAnime = false;
	
	string m_Name = {};
public:
	Node();
	//Node(Actor* Own);
	~Node();
	void SetOwner(Component* owner);
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
