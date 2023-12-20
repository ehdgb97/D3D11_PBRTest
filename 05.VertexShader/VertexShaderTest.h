#pragma once
#include <d3d11.h>
#include <directxtk/simplemath.h>

#include "../Main/MainApp.h"
class VertexShaderTest
	:public MainApp
{


public:
	VertexShaderTest(HINSTANCE hInstance);
	~VertexShaderTest();

	//D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
	//D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;
	IDXGISwapChain* m_pSwapChain = nullptr;
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;

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

	// ���̴��� �� ������ ������
	DirectX::SimpleMath::Matrix                m_World;				// ������ǥ�� �������� ��ȯ�� ���� ���.
	DirectX::SimpleMath::Matrix                m_View;				// ī�޶���ǥ�� �������� ��ȯ�� ���� ���.
	DirectX::SimpleMath::Matrix                m_Projection;		// ������ġ��ǥ��( Normalized Device Coordinate) �������� ��ȯ�� ���� ���.



	bool Initialize(UINT Width, UINT Height) override;
	void Render() override;
	bool Run() override;
	void Update() override;

	bool InitD3D();
	void UninitD3D();

	bool InitScene();		// ���̴�,���ؽ�,�ε���
	void UninitScene();
};

