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
    // 화면 칠하기.
#if USE_FLIPMODE==1
// Flip모드에서는 매프레임 설정 필요
    m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);
#endif

    m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);

    // 스왑체인 교체.
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
    ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC)); //이건 뭐지

	swapDesc.BufferCount = 1;
    //백버퍼 사이즈
    swapDesc.BufferDesc.Width = m_ClientWidth;
    swapDesc.BufferDesc.Height = m_ClientHeight;
    //화면 주사율 설정 //RefreshRate : 화면 갱신률(프레임)
	swapDesc.BufferDesc.RefreshRate.Numerator = 165;    //분자
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;    //분모
    //(float a = 0.0f~1.0f 로 0~255 가 정규화 되어있다고 생각하면된다)UNORM : Unsigned Normalize (정규화)
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = m_hWnd;
    //MSAA(Multi Sampling Anti Aliasing)쓸꺼냐? 1, 0 이면 안쓴다는 옵션.
	swapDesc.SampleDesc.Count = 1;  
	swapDesc.SampleDesc.Quality = 0;
    //이미지중에 하나를 추출하는것을 SampleDesc 라고한다.
/*
   픽셀이 적으면 계단현상이 나오는데 이것을 Aliasing이라고한다.
   그래서 그 계단현상을 없애려는 것이 Anti Aliasing이라고 한다.
   밑에 AA는 Anti Aliasing이다.
   SSAA//Super Sampling Anti Aliasing
   - 4배씩 늘리고 보정을 한 후 다시 줄인다. 그래서 선명하게 만든다.
   단점 : 비용이 너무 쎄다. 그래서 잘 안쓴다.

   MSAA//Multi Sampling Anti Aliasing (쓰레드에서 많이 쓰는 기법이다)
   - Anti Aliasing이 일어나는건 결국 테두리이다. 그래서 테두리를 잡고 보정을한다.
   단점 : 이것도 비용이 쎄다. 사용자가 이 기능을 킬 수 있게 옵션을 넣어 줄 수 있다.
*/
	swapDesc.Windowed = TRUE;;  //창모드쓸꺼냐 TRUE -> 창모드


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



    // 4. 렌더타겟뷰 생성.  (백버퍼를 이용하는 렌더타겟뷰)	
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = m_pSwapChain->GetBuffer( 0, 
        __uuidof( ID3D11Texture2D ), 
        reinterpret_cast<void**>( &pBackBuffer ) );
    if( FAILED( hr ) )
        return hr;

    hr = m_pDevice->CreateRenderTargetView( pBackBuffer, 
        nullptr, 
        &m_pRenderTargetView ); // 텍스처는 내부 참조 증가
    pBackBuffer->Release();
    pBackBuffer = nullptr;

    if( FAILED( hr ) )
        return hr;


	// 렌더 타겟을 최종 출력 파이프라인에 바인딩합니다.
	// FlipMode가 아닐때는 최초 한번만 설정하면 된다.
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
