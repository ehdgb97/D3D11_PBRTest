#pragma once
#include <windows.h>
#include "../Main/MainApp.h"
#include <d3d11.h>
#include <directxtk/simplemath.h>

using namespace DirectX::SimpleMath;
using namespace DirectX;

struct CB_Transform
{
	Matrix mWorld;
	Matrix mView;
	Matrix mProjection;
};
struct CB_DirectionLight
{
	Vector3 Direction={0.f,0.f,1.f};
	float pad0=0;
	Vector4 Ambient = { 0.f,0.f,0.f,1.f };
	Vector4 Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	Vector4 Specular = { 1.0f,1.0f,1.0f,1.0f };
	Vector3 EyePosition;
	bool UseBlinnPhong = false;
	bool pad1[3];
};
struct CB_Material
{
	Vector4 Ambient = { 1.0f,1.0f,1.0f,1.0f };
	Vector4 Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	Vector4 Specular = { 1.0f,1.0f,1.0f,1.0f };
	float  SpecularPower = 128.f;
	bool UseNormalMap = true;
	bool pad0[3];
	bool UseSpecularMap = true;
	bool pad1[3];
	float pad2;
};


class MapTest
	:public MainApp
{

public:
	MapTest(HINSTANCE hInstance);
	~MapTest();

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
	ID3D11DepthStencilView* m_pDepthStencilView = nullptr;  // 깊이값 처리를 위한 뎊스스텐실 뷰

	CB_Transform m_transformData;
	CB_DirectionLight m_directionLightData;
	CB_Material m_materialData;

	ID3D11Buffer* m_pTransformCB = nullptr;				// 상수 버퍼.
	ID3D11Buffer* m_pDirectionLightCB = nullptr;		// 상수 버퍼.
	ID3D11Buffer* m_pMaterialCB = nullptr;				// 상수 버퍼.

	ID3D11ShaderResourceView* m_pTextureRV = nullptr;	// 텍스처 리소스 뷰.
	ID3D11ShaderResourceView* m_pNormalRV = nullptr;	// 노멀 리소스 뷰.
	ID3D11ShaderResourceView* m_pSpecularRV = nullptr;	// 스펙큘러 리소스 뷰.

	ID3D11SamplerState* m_pSamplerLinear = nullptr;		// 샘플러 상태.


	Vector3 m_cubePos;
	Vector3 m_cubesScale;
	Vector3 m_cubeAngle;
	Vector3 m_cubeRotationAngle;
	bool m_cubeRotationUse;
	Vector3 m_cameraTrans;
	Vector3 m_cameraRot;
	Vector2 m_cameraDistance;
	float	m＿cameraFov;

	// 쉐이더에 에 전달할 데이터
	Matrix     m_World;	// 월드좌표계 공간으로 변환을 위한 행렬.
	Matrix     m_View;			// 카메라좌표계 공간으로 변환을 위한 행렬.
	Matrix     m_Projection;	// 단위장치좌표계( Normalized Device Coordinate) 공간으로 변환을 위한 행렬.
	Matrix     m_World1;	// 월드좌표계 공간으로 변환을 위한 행렬.





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

