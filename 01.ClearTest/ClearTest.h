#pragma once
#include <d3d11.h>
#include "../Main/MainApp.h"

class ClearTest
	:public MainApp
{
public:
	ClearTest(HINSTANCE hInstance);
	~ClearTest();



	//D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
	//D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;
	IDXGISwapChain* m_pSwapChain = nullptr;
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;

	bool Initialize(UINT Width, UINT Height) override;
	void Render() override;
	bool Run() override;
	void Update() override;




	bool InitD3D();
	void UninitD3D();
};

