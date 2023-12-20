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




	// ������ ���������ο� �����ϴ�  ��ü�� ����
	ID3D11VertexShader* m_pVertexShader = nullptr;	// ���� ���̴�.
	ID3D11PixelShader* m_pPixelShader = nullptr;	// �ȼ� ���̴�.	
	ID3D11InputLayout* m_pInputLayout = nullptr;	// �Է� ���̾ƿ�.
	ID3D11Buffer* m_pVertexBuffer = nullptr;		// ���ؽ� ����.
	UINT m_VertextBufferStride = 0;					// ���ؽ� �ϳ��� ũ��.
	UINT m_VertextBufferOffset = 0;					// ���ؽ� ������ ������.
	ID3D11Buffer* m_pIndexBuffer = nullptr;			// ���ؽ� ����.
	int m_nIndices = 0;								// �ε��� ����.
	ID3D11DepthStencilView* m_pDepthStencilView = nullptr;  // ���̰� ó���� ���� �X�����ٽ� ��

	CB_Transform m_transformData;
	CB_DirectionLight m_directionLightData;
	CB_Material m_materialData;

	ID3D11Buffer* m_pTransformCB = nullptr;				// ��� ����.
	ID3D11Buffer* m_pDirectionLightCB = nullptr;		// ��� ����.
	ID3D11Buffer* m_pMaterialCB = nullptr;				// ��� ����.

	ID3D11ShaderResourceView* m_pTextureRV = nullptr;	// �ؽ�ó ���ҽ� ��.
	ID3D11ShaderResourceView* m_pNormalRV = nullptr;	// ��� ���ҽ� ��.
	ID3D11ShaderResourceView* m_pSpecularRV = nullptr;	// ����ŧ�� ���ҽ� ��.

	ID3D11SamplerState* m_pSamplerLinear = nullptr;		// ���÷� ����.


	Vector3 m_cubePos;
	Vector3 m_cubesScale;
	Vector3 m_cubeAngle;
	Vector3 m_cubeRotationAngle;
	bool m_cubeRotationUse;
	Vector3 m_cameraTrans;
	Vector3 m_cameraRot;
	Vector2 m_cameraDistance;
	float	m��cameraFov;

	// ���̴��� �� ������ ������
	Matrix     m_World;	// ������ǥ�� �������� ��ȯ�� ���� ���.
	Matrix     m_View;			// ī�޶���ǥ�� �������� ��ȯ�� ���� ���.
	Matrix     m_Projection;	// ������ġ��ǥ��( Normalized Device Coordinate) �������� ��ȯ�� ���� ���.
	Matrix     m_World1;	// ������ǥ�� �������� ��ȯ�� ���� ���.





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

