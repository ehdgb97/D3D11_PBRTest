#pragma once
#include "ConstantBuffers.h"


struct CB_Transform;
using namespace DirectX::SimpleMath;
using namespace std;

struct aiMaterial;


class Material
{
public:
	Material();
	~Material();
	CB_Material m_MaterialCB;

	
	void Create(aiMaterial* pMaterial);
	void Render(ID3D11DeviceContext* m_pDeviceContext, ID3D11BlendState* m_pAlphaBlendState, ID3D11Buffer* m_pMaterialCB);
	ID3D11ShaderResourceView* m_pDiffuseRV = nullptr;	// �ؽ�ó ���ҽ� ��.
	ID3D11ShaderResourceView* m_pNormalRV = nullptr;	// �븻�� ���ҽ� ��.
	ID3D11ShaderResourceView* m_pSpecularRV = nullptr;	// ����ŧ���� ���ҽ� ��.
	ID3D11ShaderResourceView* m_pEmissiveRV = nullptr;	// �̹̽ú�� ���ҽ� ��.	
	ID3D11ShaderResourceView* m_pOpacityRV = nullptr;	// ����� ���ҽ� ��.	
	ID3D11ShaderResourceView* m_pMetalnessRV = nullptr;	// ��Ż�� ���ҽ� ��.	
	ID3D11ShaderResourceView* m_pRoughnessRV = nullptr;	// ��Ż�� ���ҽ� ��.	

};

