#include "ClearTest.h"
#include <directxtk/simplemath.h>
#include "../Main/Helper.h"
#pragma comment (lib, "d3d11.lib")

ClearTest::ClearTest(HINSTANCE hInstance)
	:MainApp(hInstance)
{
}

ClearTest::~ClearTest()
{
	UninitD3D();
}

bool ClearTest::Initialize(UINT Width, UINT Height)
{
	 MainApp::Initialize(Width, Height);
     InitD3D();
	 return true;
}

void ClearTest::Render()
{
	DirectX::SimpleMath::Color color(0.0f, 0.5f, 0.5f, 1.0f);
    // ȭ�� ĥ�ϱ�.
#if USE_FLIPMODE==1
// Flip��忡���� �������� ���� �ʿ�
    m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);
#endif

    m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);

    // ����ü�� ��ü.
    m_pSwapChain->Present(0, 0);
}

bool ClearTest::Run()
{
	 MainApp::Run();
	 return true;
}

void ClearTest::Update()
{
	MainApp::Update();
}

bool ClearTest::InitD3D()
{
	HRESULT hr = S_OK;


	// DirectX 11.0 systems
	DXGI_SWAP_CHAIN_DESC swapDesc = {};
    ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC)); //�̰� ����

	swapDesc.BufferCount = 1;
    //����� ������
    swapDesc.BufferDesc.Width = m_ClientWidth;
    swapDesc.BufferDesc.Height = m_ClientHeight;
    //ȭ�� �ֻ��� ���� //RefreshRate : ȭ�� ���ŷ�(������)
	swapDesc.BufferDesc.RefreshRate.Numerator = 165;    //����
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;    //�и�
    //(float a = 0.0f~1.0f �� 0~255 �� ����ȭ �Ǿ��ִٰ� �����ϸ�ȴ�)UNORM : Unsigned Normalize (����ȭ)
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = m_hWnd;
    //MSAA(Multi Sampling Anti Aliasing)������? 1, 0 �̸� �Ⱦ��ٴ� �ɼ�.
	swapDesc.SampleDesc.Count = 1;  
	swapDesc.SampleDesc.Quality = 0;
    //�̹����߿� �ϳ��� �����ϴ°��� SampleDesc ����Ѵ�.
/*
   �ȼ��� ������ ��������� �����µ� �̰��� Aliasing�̶���Ѵ�.
   �׷��� �� ��������� ���ַ��� ���� Anti Aliasing�̶�� �Ѵ�.
   �ؿ� AA�� Anti Aliasing�̴�.
   SSAA//Super Sampling Anti Aliasing
   - 4�辿 �ø��� ������ �� �� �ٽ� ���δ�. �׷��� �����ϰ� �����.
   ���� : ����� �ʹ� ���. �׷��� �� �Ⱦ���.

   MSAA//Multi Sampling Anti Aliasing (�����忡�� ���� ���� ����̴�)
   - Anti Aliasing�� �Ͼ�°� �ᱹ �׵θ��̴�. �׷��� �׵θ��� ��� �������Ѵ�.
   ���� : �̰͵� ����� ���. ����ڰ� �� ����� ų �� �ְ� �ɼ��� �־� �� �� �ִ�.
*/
	swapDesc.Windowed = TRUE;;  //â��徵���� TRUE -> â���


    UINT creationFlags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#ifdef _DEBUG
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif



    hr = D3D11CreateDeviceAndSwapChain(NULL, 
        D3D_DRIVER_TYPE_HARDWARE, 
        NULL, 
        creationFlags, 
        NULL, 
        NULL,
        D3D11_SDK_VERSION, 
        &swapDesc, 
        &m_pSwapChain, 
        &m_pDevice, 
        NULL, 
        &m_pDeviceContext);

    if (FAILED(hr))
        return hr;



    // 4. ����Ÿ�ٺ� ����.  (����۸� �̿��ϴ� ����Ÿ�ٺ�)	
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = m_pSwapChain->GetBuffer( 0, 
        __uuidof( ID3D11Texture2D ), 
        reinterpret_cast<void**>( &pBackBuffer ) );
    if( FAILED( hr ) )
        return hr;

    hr = m_pDevice->CreateRenderTargetView( pBackBuffer, 
        nullptr, 
        &m_pRenderTargetView ); // �ؽ�ó�� ���� ���� ����
    pBackBuffer->Release();
    pBackBuffer = nullptr;

    if( FAILED( hr ) )
        return hr;


	// ���� Ÿ���� ���� ��� ���������ο� ���ε��մϴ�.
	// FlipMode�� �ƴҶ��� ���� �ѹ��� �����ϸ� �ȴ�.
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);

    return S_OK;
}

void ClearTest::UninitD3D()
{
    m_pRenderTargetView->Release();
	m_pRenderTargetView = nullptr;
    m_pDeviceContext->Release();
	m_pDeviceContext = nullptr;
    m_pSwapChain->Release();
	m_pSwapChain = nullptr;
    m_pDevice->Release();
	m_pDevice = nullptr;
}
