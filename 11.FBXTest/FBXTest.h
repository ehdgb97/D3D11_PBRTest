#pragma once
#include "../Main/MainApp.h"
#include "../Main/GameObject.h"
#include "../Main/ConstantBuffers.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;


//struct CB_Material
//{
//	Vector4 Ambient = { 1.0f,1.0f,1.0f,1.0f };
//	Vector4 Diffuse = { 1.0f,1.0f,1.0f,1.0f };
//	Vector4 Specular = { 1.0f,1.0f,1.0f,1.0f };
//	Vector4 Emissive = { 1.0f,1.0f,1.0f,1.0f };
//	float  SpecularPower = 128.f; //4
//	bool Use_DiffuseMap = false;
//	bool pad0[3]={};					//8			
//	bool Use_NormalMap = false;
//	bool pad1[3] = {};//12
//	bool Use_SpecularMap = false;
//	bool pad2[3] = {};//16
//	bool Use_EmissiveMap = false;
//	bool pad3[3] = {};//4
//	bool Use_OpacityMap = false;
//	bool pad4[3] = {};//8
//	Vector2 pad5;//16
//};


class FBXTest
	:public MainApp
{

public:
	FBXTest(HINSTANCE hInstance);
	~FBXTest();

	// ������ ������������ �����ϴ� �ʼ� ��ü�� �������̽�
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;
	IDXGISwapChain* m_pSwapChain = nullptr;
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
	ID3D11DepthStencilView* m_pDepthStencilView = nullptr;  // ���̰� ó���� ���� �X�����ٽ� ��


	// ������ ���������ο� �����ϴ�  ��ü�� ����
	ID3D11VertexShader* m_pVertexShader = nullptr;		// ���� ���̴�.
	ID3D11PixelShader* m_pPixelShader = nullptr;		// �ȼ� ���̴�.	
	ID3D11InputLayout* m_pInputLayout = nullptr;		// �Է� ���̾ƿ�.
	ID3D11SamplerState* m_pSamplerLinear = nullptr;		// ���÷� ����.
	ID3D11BlendState* m_pAlphaBlendState = nullptr;		// ���� ���� ���� (������ó�������� �������)
	ID3D11Buffer* m_pTransformCB = nullptr;				// ��� ����.
	ID3D11Buffer* m_pDirectionLightCB = nullptr;		// ��� ����.
	ID3D11Buffer* m_pMaterialCB = nullptr;				// ��� ����.




	// ���̴��� �� ������ ������
		// ������ ���������ο� �����ϴ� ����

	Matrix     m_World;			// ������ǥ�� �������� ��ȯ�� ���� ���.
	Matrix     m_View;			// ī�޶���ǥ�� �������� ��ȯ�� ���� ���.
	Matrix     m_Projection;	// ������ġ��ǥ��( Normalized Device Coordinate) �������� ��ȯ�� ���� ���.

	Vector3 m_cameraTrans;
	Vector3 m_cameraRot;
	Vector2 m_cameraDistance;
	float	m��cameraFov;

	CB_Transform m_transformCB;
	CB_DirectionLight m_directionLightCB;
	CB_Material m_materialCB;

	vector<GameObject*> mGameobject;

	Vector3 m_meshRotationAngle;
	bool m_meshRotationUse;


	bool Initialize(UINT Width, UINT Height) override;
	void Render() override;
	bool Run() override;
	void Update() override;

	bool InitD3D();
	bool InitScene();		// ���̴�,���ؽ�,�ε���
	bool InitImGUI();

	void UninitD3D();
	void UninitScene();
	void UninitImGUI();
	virtual LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};

