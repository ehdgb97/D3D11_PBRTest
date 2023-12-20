#pragma once
#include <d3d11.h>
#include <vector>
#include <directxtk/simplemath.h>
#include "../Main/MainApp.h"

struct CB_Transform
{
	DirectX::SimpleMath::Matrix mWorld;
	DirectX::SimpleMath::Matrix mView;
	DirectX::SimpleMath::Matrix mProjection;
	DirectX::SimpleMath::Vector4 vLightDir;
	DirectX::SimpleMath::Vector4 vLightColor;
	DirectX::SimpleMath::Vector4 vOutputColor;

	//int a;
};

class LightTest
	:public MainApp
{

public:
	LightTest(HINSTANCE hInstance);
	~LightTest();

	//D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
	//D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;
	IDXGISwapChain* m_pSwapChain = nullptr;
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;


	///Imgui
	bool m_show_demo_window = true;
	CB_Transform m_transformData1;
	CB_Transform m_transformData2;
	CB_Transform m_transformData3;
	float m_firstCubeRotation[6];
	DirectX::XMFLOAT3 m_firstCubePos;
	DirectX::XMFLOAT3 m_secondCubePos;
	DirectX::XMFLOAT3 m_thirdCubePos;

	DirectX::XMFLOAT3 m_cubesScale;
	DirectX::XMFLOAT3 m_cameraTrans;
	DirectX::XMFLOAT3 m_cameraRot;
	float m＿cameraFov;
	DirectX::XMFLOAT2 m_cameraDistance;

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
	ID3D11DepthStencilView* m_pDepthStencilView = nullptr;  // 깊이값 처리를 위한 뎊스스텐실 뷰
	float m_angle;
	float m_plusangle;

	// 쉐이더에 에 전달할 데이터
	DirectX::SimpleMath::Matrix     m_MainWorld;	// 월드좌표계 공간으로 변환을 위한 행렬.
	DirectX::SimpleMath::Matrix     m_World1;	// 월드좌표계 공간으로 변환을 위한 행렬.

	DirectX::SimpleMath::Matrix     m_World2;		// 월드좌표계 공간으로 변환을 위한 행렬.
	DirectX::SimpleMath::Matrix     m_World3;		// 월드좌표계 공간으로 변환을 위한 행렬.
	DirectX::SimpleMath::Matrix     m_View;			// 카메라좌표계 공간으로 변환을 위한 행렬.
	DirectX::SimpleMath::Matrix     m_Projection;	// 단위장치좌표계( Normalized Device Coordinate) 공간으로 변환을 위한 행렬.
	DirectX::XMFLOAT4 m_LightColor =
	{
		DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f)
	};
	DirectX::XMFLOAT4 m_InitialLightDirs =
	{
		DirectX::XMFLOAT4(0.0f, 0.0f, -1.0f, 1.0f),
	};
	DirectX::XMFLOAT4 m_LightDirsEvaluated = {};		// 계산된 라이트 방향



	bool Initialize(UINT Width, UINT Height) override;
	void Render() override;
	bool Run() override;
	void Update() override;

	bool InitD3D();
	bool InitScene();		// 쉐이더,버텍스,인덱스
	bool InitImGUI();

	void UninitD3D();
	void UninitScene();
	void UninitImGUI();
	virtual LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};

