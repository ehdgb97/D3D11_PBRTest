#pragma once
#include <windows.h>
#include "../Main/MainApp.h"
#include <d3d11.h>
#include <directxtk/simplemath.h>

struct ConstantBuffer
{
	DirectX::SimpleMath::Matrix mWorld;
	DirectX::SimpleMath::Matrix mView;
	DirectX::SimpleMath::Matrix mProjection;
	DirectX::SimpleMath::Vector4 vLightDir;
	DirectX::SimpleMath::Vector4 vLightColor;
	DirectX::SimpleMath::Vector4 vOutputColor;

	//int a;
};

class TextureTest
	:public MainApp
{

public:
	TextureTest(HINSTANCE hInstance);
	~TextureTest();

	//D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
	//D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;
	IDXGISwapChain* m_pSwapChain = nullptr;
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;


	///Imgui
	bool m_show_demo_window = true;
	ConstantBuffer m_transformData1;
	ConstantBuffer m_transformData2;
	ConstantBuffer m_transformData3;
	float m_firstCubeRotation[6];
	DirectX::XMFLOAT3 m_firstCubePos;
	DirectX::XMFLOAT3 m_secondCubePos;
	DirectX::XMFLOAT3 m_thirdCubePos;

	DirectX::XMFLOAT3 m_cubesScale;
	DirectX::XMFLOAT3 m_cameraTrans;
	DirectX::XMFLOAT3 m_cameraRot;
	float m��cameraFov;
	DirectX::XMFLOAT2 m_cameraDistance;

	// ������ ���������ο� �����ϴ�  ��ü�� ����
	ID3D11VertexShader* m_pVertexShader = nullptr;	// ���� ���̴�.
	ID3D11PixelShader* m_pPixelShader = nullptr;	// �ȼ� ���̴�.	
	ID3D11InputLayout* m_pInputLayout = nullptr;	// �Է� ���̾ƿ�.
	ID3D11Buffer* m_pVertexBuffer = nullptr;		// ���ؽ� ����.
	UINT m_VertextBufferStride = 0;					// ���ؽ� �ϳ��� ũ��.
	UINT m_VertextBufferOffset = 0;					// ���ؽ� ������ ������.
	ID3D11Buffer* m_pIndexBuffer = nullptr;			// ���ؽ� ����.
	int m_nIndices = 0;								// �ε��� ����.
	ID3D11Buffer* m_pConstantBuffer = nullptr;		// ��� ����.
	ID3D11DepthStencilView* m_pDepthStencilView = nullptr;  // ���̰� ó���� ���� �X�����ٽ� ��
	DirectX::XMFLOAT3 m_angle;
	/*float m_angle;*/
	DirectX::XMFLOAT3 m_plusangle;

	ID3D11ShaderResourceView* m_pTextureRV = nullptr;	// �ؽ�ó ���ҽ� ��.
	ID3D11SamplerState* m_pSamplerLinear = nullptr;		// ���÷� ����.



	// ���̴��� �� ������ ������
	DirectX::SimpleMath::Matrix     m_MainWorld;	// ������ǥ�� �������� ��ȯ�� ���� ���.
	DirectX::SimpleMath::Matrix     m_World1;	// ������ǥ�� �������� ��ȯ�� ���� ���.

	DirectX::SimpleMath::Matrix     m_World2;		// ������ǥ�� �������� ��ȯ�� ���� ���.
	DirectX::SimpleMath::Matrix     m_World3;		// ������ǥ�� �������� ��ȯ�� ���� ���.
	DirectX::SimpleMath::Matrix     m_View;			// ī�޶���ǥ�� �������� ��ȯ�� ���� ���.
	DirectX::SimpleMath::Matrix     m_Projection;	// ������ġ��ǥ��( Normalized Device Coordinate) �������� ��ȯ�� ���� ���.
	DirectX::XMFLOAT4 m_LightColor =
	{
		DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
	};
	DirectX::XMFLOAT4 m_InitialLightDirs =
	{
		DirectX::XMFLOAT4(0.0f, 0.0f, -1.0f, 1.0f),
	};
	DirectX::XMFLOAT4 m_LightDirsEvaluated = {};		// ���� ����Ʈ ����



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

