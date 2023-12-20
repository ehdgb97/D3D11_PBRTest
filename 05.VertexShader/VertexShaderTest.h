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

	// 렌더링 파이프라인에 적용하는  객체와 정보
	ID3D11VertexShader* m_pVertexShader = nullptr;	// 정점 셰이더.
	ID3D11PixelShader* m_pPixelShader = nullptr;	// 픽셀 셰이더.	
	ID3D11InputLayout* m_pInputLayout = nullptr;	// 입력 레이아웃.
	ID3D11Buffer* m_pVertexBuffer = nullptr;		// 버텍스 버퍼.
	UINT m_VertextBufferStride = 0;					// 버텍스 하나의 크기.
	UINT m_VertextBufferOffset = 0;					// 버텍스 버퍼의 오프셋.
	ID3D11Buffer* m_pIndexBuffer = nullptr;			// 버텍스 버퍼.
	int m_nIndices = 0;								// 인덱스 개수.

	ID3D11Buffer* m_pConstantBuffer = nullptr;		// 상수 버퍼.

	// 쉐이더에 에 전달할 데이터
	DirectX::SimpleMath::Matrix                m_World;				// 월드좌표계 공간으로 변환을 위한 행렬.
	DirectX::SimpleMath::Matrix                m_View;				// 카메라좌표계 공간으로 변환을 위한 행렬.
	DirectX::SimpleMath::Matrix                m_Projection;		// 단위장치좌표계( Normalized Device Coordinate) 공간으로 변환을 위한 행렬.



	bool Initialize(UINT Width, UINT Height) override;
	void Render() override;
	bool Run() override;
	void Update() override;

	bool InitD3D();
	void UninitD3D();

	bool InitScene();		// 쉐이더,버텍스,인덱스
	void UninitScene();
};

