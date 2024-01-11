#pragma once
#pragma once
#include "ConstantBuffers.h"
#include "TimeSystem.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;
class Actor;
 
class D3DRenderManager
{

public:
	D3DRenderManager();
	~D3DRenderManager();

	static D3DRenderManager* Instance;
	static ID3D11Device* m_pDevice;						// ����̽�
	// ������ ������������ �����ϴ� �ʼ� ��ü�� �������̽�
	//ID3D11Device* m_pDevice = nullptr;
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
	ID3D11Buffer* m_pMatrixPalleteCB = nullptr;			// ��� ����

	D3D11_VIEWPORT m_viewport;

	// ���̴��� �� ������ ������
		// ������ ���������ο� �����ϴ� ����

	Matrix     m_World;			// ������ǥ�� �������� ��ȯ�� ���� ���.
	Matrix     m_View;			// ī�޶���ǥ�� �������� ��ȯ�� ���� ���.
	Matrix     m_Projection;	// ������ġ��ǥ��( Normalized Device Coordinate) �������� ��ȯ�� ���� ���.

	Vector3 m_cameraTrans;
	Vector3 m_cameraRot;
	Vector2 m_cameraDistance;
	float	m��cameraFov;

	CB_Transform m_TransformCB;
	CB_DirectionLight m_DirectionLightCB;
	CB_Material m_MaterialCB;

	HWND m_hWnd = nullptr;

	std::list<std::shared_ptr<Actor>> m_Actors;	// ������ ���� ������Ʈ��	

	Vector3 m_meshRotationAngle;
	bool m_meshRotationUse;


	bool Initialize(HWND Handle,UINT Width, UINT Height);
	void Render();
	bool Run();
	void Update();

	bool InitD3D(UINT ClientWidth, UINT ClientHeight);
	bool InitScene();		// ���̴�,���ؽ�,�ε���
	bool InitImGUI();

	void UninitD3D();
	void UninitScene();
	void UninitImGUI();
	//virtual LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};

