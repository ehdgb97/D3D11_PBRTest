#pragma once


#include "Animation.h"
#include <string>
#include "ConstantBuffers.h"

class Node;
class Material;
class Mesh;
class BoneReference;
class BoneWeightVertex;

class GameObject
{
	Matrix mLocalTransformMatrix;
	Matrix     mWorld;		// 월드좌표계 공간으로 변환을 위한 행렬.
	CB_Material m_MaterialCB;
	vector< Mesh*> m_Meshes;
	vector< Material*> m_Materials;
	shared_ptr<Node> m_rootNode;
	vector<Node*> m_Nodes;
	vector <Animation*> m_pAnimation;
	Vector3 m_position = {};
	Vector3 m_scale = {.3f,.3f,.3f};
	Vector3 m_angle={};
	bool m_useAnime = false;
public:
	unsigned int AnimationIndex=0;
	GameObject();
	~GameObject();
	bool Update(ID3D11DeviceContext* deviceContext, ID3D11Buffer* m_pBoneTransformBuffer);
	bool Render(ID3D11DeviceContext* m_pDeviceContext, ID3D11BlendState* m_pAlphaBlendState, ID3D11Buffer* m_pMaterialCB, ID3D11Buffer* m_pTransformCB, ID3D11Buffer* m_pBoneTransformBuffer,CB_Transform* test);


	bool SetFBX( string Filename);
	void SetWorld(Matrix world);
	void SetPos(Vector3 Pos);
	void SetScale(Vector3 Scale);
	void SetAngle(Vector3 Angle);

	// Setter for m_rootNode
	void SetRootNode(const shared_ptr<Node>& rootNode);
	void SetMaterialCB(CB_Material materialcb);
	shared_ptr<Node> GetRootNode() const;
	Matrix GetWorld();
	Vector3 GetPos();
	Vector3 GetScale();
	Vector3 GetAngle();
	CB_Material GetMaterialCB();
	const vector<Mesh*>& GetMeshes() const;
	const vector< Material*>& GetMaterials() const;
	vector<Node*>& GetNodes();
	void SetUseAni(bool Use);
	bool GetUseAni();
	const vector<Animation*>& GetAnimations() const;
};

inline void GameObject::SetUseAni(bool Use)
{
	m_useAnime = Use;
}

inline bool GameObject::GetUseAni()
{
	return m_useAnime;
}

inline void GameObject::SetWorld(Matrix world)
{
	mWorld = mLocalTransformMatrix*world;
}

inline void GameObject::SetPos(Vector3 Pos)
{
	m_position = Pos;
}
inline void GameObject::SetScale(Vector3 Scale)
{
	m_scale = Scale;
}
inline void GameObject::SetAngle(Vector3 Angle)
{
	m_angle = Angle;
}
inline Vector3 GameObject::GetPos()
{
	return m_position;
}
inline Vector3 GameObject::GetScale()
{
	return m_scale;
}
inline Vector3 GameObject::GetAngle()
{
	return m_angle;
}
inline const vector<Mesh*>& GameObject::GetMeshes() const
{
	return m_Meshes;
}
inline const vector<Material*>& GameObject::GetMaterials() const
{
	return m_Materials;
}
inline vector<Node*>& GameObject::GetNodes()
{
	return m_Nodes;
}
inline const vector<Animation*>& GameObject::GetAnimations() const
{
	return m_pAnimation;
}
