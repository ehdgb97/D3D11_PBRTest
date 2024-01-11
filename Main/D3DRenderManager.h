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
	static ID3D11Device* m_pDevice;						// 디바이스
	// 렌더링 파이프라인을 구성하는 필수 객체의 인터페이스
	//ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;
	IDXGISwapChain* m_pSwapChain = nullptr;
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
	ID3D11DepthStencilView* m_pDepthStencilView = nullptr;  // 깊이값 처리를 위한 뎊스스텐실 뷰


	// 렌더링 파이프라인에 적용하는  객체와 정보
	ID3D11VertexShader* m_pVertexShader = nullptr;		// 정점 셰이더.
	ID3D11PixelShader* m_pPixelShader = nullptr;		// 픽셀 셰이더.	
	ID3D11InputLayout* m_pInputLayout = nullptr;		// 입력 레이아웃.
	ID3D11SamplerState* m_pSamplerLinear = nullptr;		// 샘플러 상태.
	ID3D11BlendState* m_pAlphaBlendState = nullptr;		// 블렌드 상태 변경 (반투명처리를위한 블렌드상태)

	ID3D11Buffer* m_pTransformCB = nullptr;				// 상수 버퍼.
	ID3D11Buffer* m_pDirectionLightCB = nullptr;		// 상수 버퍼.
	ID3D11Buffer* m_pMaterialCB = nullptr;				// 상수 버퍼.
	ID3D11Buffer* m_pMatrixPalleteCB = nullptr;			// 상수 버퍼

	D3D11_VIEWPORT m_viewport;

	// 쉐이더에 에 전달할 데이터
		// 렌더링 파이프라인에 적용하는 정보

	Matrix     m_World;			// 월드좌표계 공간으로 변환을 위한 행렬.
	Matrix     m_View;			// 카메라좌표계 공간으로 변환을 위한 행렬.
	Matrix     m_Projection;	// 단위장치좌표계( Normalized Device Coordinate) 공간으로 변환을 위한 행렬.

	Vector3 m_cameraTrans;
	Vector3 m_cameraRot;
	Vector2 m_cameraDistance;
	float	m＿cameraFov;

	CB_Transform m_TransformCB;
	CB_DirectionLight m_DirectionLightCB;
	CB_Material m_MaterialCB;

	HWND m_hWnd = nullptr;

	std::list<std::shared_ptr<Actor>> m_Actors;	// 생성된 게임 오브젝트들	

	Vector3 m_meshRotationAngle;
	bool m_meshRotationUse;


	bool Initialize(HWND Handle,UINT Width, UINT Height);
	void Render();
	bool Run();
	void Update();

	bool InitD3D(UINT ClientWidth, UINT ClientHeight);
	bool InitScene();		// 쉐이더,버텍스,인덱스
	bool InitImGUI();

	void UninitD3D();
	void UninitScene();
	void UninitImGUI();
	//virtual LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};

