#include "DrawTriangle.h"
#include <directxtk/simplemath.h>
#include <d3dcompiler.h>
#include "../Main/Helper.h"
#pragma comment (lib, "d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

struct Vertex
{
	DirectX::SimpleMath::Vector3 position;		// 위치 정보.
};


DrawTriangle::DrawTriangle(HINSTANCE hInstance)
    :MainApp(hInstance)
{
}

DrawTriangle::~DrawTriangle()
{
	UninitScene();
    UninitD3D();
}

bool DrawTriangle::Initialize(UINT Width, UINT Height)
{
    MainApp::Initialize(Width, Height);
    if (!InitD3D())
        return false;

    if (!InitScene())
        return false;
    return true;
}

void DrawTriangle::Render()
{
    float color[4] = { 0.0f, 0.5f, 0.5f, 1.0f };

    // 화면 칠하기.
    m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);

    // Draw계열 함수를 호출하기전에 렌더링 파이프라인에 필수 스테이지 설정을 해야한다.	
    m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 정점을 이어서 그릴 방식 설정.
    m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_VertextBufferStride, &m_VertextBufferOffset);
    m_pDeviceContext->IASetInputLayout(m_pInputLayout);
    m_pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
    m_pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);

    // Render a triangle	
    m_pDeviceContext->Draw(m_VertexCount, 0);

    // Present the information rendered to the back buffer to the front buffer (the screen)
    m_pSwapChain->Present(0, 0);

}

bool DrawTriangle::Run()
{
    MainApp::Run();
    return true;
}

void DrawTriangle::Update()
{
    MainApp::Update();
}


bool DrawTriangle::InitD3D()
{
    HRESULT hr = 0;

    // DirectX 11.0 systems
    DXGI_SWAP_CHAIN_DESC swapDesc = {};
    ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC)); //이건 뭐지

    swapDesc.BufferCount = 1;
    //백버퍼 사이즈
    swapDesc.BufferDesc.Width = m_ClientWidth;
    swapDesc.BufferDesc.Height = m_ClientHeight;
    //화면 주사율 설정 //RefreshRate : 화면 갱신률(프레임)
    swapDesc.BufferDesc.RefreshRate.Numerator = 60;    //분자
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



    HR_T(D3D11CreateDeviceAndSwapChain(NULL,
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
        &m_pDeviceContext));


    // 4. 렌더타겟뷰 생성.  (백버퍼를 이용하는 렌더타겟뷰)	
    ID3D11Texture2D* pBackBuffer = nullptr;
    HR_T(m_pSwapChain->GetBuffer(0,
        __uuidof(ID3D11Texture2D),
        reinterpret_cast<void**>(&pBackBuffer)));


    HR_T(m_pDevice->CreateRenderTargetView(pBackBuffer,
        nullptr,
        &m_pRenderTargetView)); // 텍스처는 내부 참조 증가
    SAFE_RELEASE(pBackBuffer);

    m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);
    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = (float)m_ClientWidth;
    viewport.Height = (float)m_ClientHeight;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    m_pDeviceContext->RSSetViewports(1, &viewport);


    return true;
    
}

void DrawTriangle::UninitD3D()
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

bool DrawTriangle::InitScene()
{
    using namespace DirectX::SimpleMath;
    HRESULT hr = 0; // 결과값.
    ID3D10Blob* errorMessage = nullptr;	 // 컴파일 에러 메시지가 저장될 버퍼.	

    //1. Render() 에서 파이프라인에 바인딩할 버텍스 버퍼및 버퍼 정보 준비
    // 아직은 VertexShader의 World, View, Projection 변환을 사용하지 않으므로 
    // 직접 Normalized Device Coordinate(좌표계)의 위치로 설정한다.
    //      /---------------------(1,1,1)   z값은 깊이값
    //     /                      / |   
    // (-1,1,0)----------------(1,1,0)        
    //   |         v1           |   |
    //   |        /   `         |   |       중앙이 (0,0,0)  
    //   |       /  +   `       |   |
    //   |     /         `      |   |
    //	 |   v0-----------v2    |  /
    // (-1,-1,0)-------------(1,-1,0)
    Vertex vertices[] =
    {
    	Vector3(-0.5,0.5,0.5), // v0    
        Vector3(0.5,0.5,0.5),		// v1    
        Vector3(-0.5,-0.5,0.5),	// v2
        Vector3(-0.5,-0.5,0.5),	// v2
        Vector3(0.5,0.5,0.5),		// v1    
        Vector3(0.5,-0.5,0.5),		// v1    
    };

    D3D11_BUFFER_DESC vbDesc = {};
    m_VertexCount = ARRAYSIZE(vertices);
    vbDesc.ByteWidth = sizeof(Vertex) * m_VertexCount;
    vbDesc.CPUAccessFlags = 0;
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbDesc.MiscFlags = 0;
    vbDesc.Usage = D3D11_USAGE_DEFAULT;

    // 정점 버퍼 생성.
    D3D11_SUBRESOURCE_DATA vbData = {};
    vbData.pSysMem = vertices;
    HR_T(m_pDevice->CreateBuffer(&vbDesc, &vbData, &m_pVertexBuffer));

    // 버텍스 버퍼 정보 
    m_VertextBufferStride = sizeof(Vertex);
    m_VertextBufferOffset = 0;


    // 2. Render() 에서 파이프라인에 바인딩할 InputLayout 생성 	
    D3D11_INPUT_ELEMENT_DESC layout[] =  // 인풋 레이아웃은 버텍스 쉐이더가 입력받을 데이터의 형식을 지정한다.
    {// SemanticName , SemanticIndex , Format , InputSlot , AlignedByteOffset , InputSlotClass , InstanceDataStepRate		
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    ID3DBlob* vertexShaderBuffer = nullptr;
    HR_T(CompileShaderFromFile(L"BasicVertexShader.hlsl", "main", "vs_4_0", &vertexShaderBuffer));
    HR_T( m_pDevice->CreateInputLayout(layout, ARRAYSIZE(layout),vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_pInputLayout));


    // 3. Render에서 파이프라인에 바인딩할  버텍스 셰이더 생성
    HR_T(m_pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),vertexShaderBuffer->GetBufferSize(), NULL, &m_pVertexShader));
    SAFE_RELEASE(vertexShaderBuffer);


    // 4. Render에서 파이프라인에 바인딩할 픽셀 셰이더 생성
    ID3DBlob* pixelShaderBuffer = nullptr;
    HR_T(CompileShaderFromFile(L"BasicPixelShader.hlsl", "main", "ps_4_0", &pixelShaderBuffer));
    HR_T(m_pDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),pixelShaderBuffer->GetBufferSize(), NULL, &m_pPixelShader));
    SAFE_RELEASE(pixelShaderBuffer);

    return true;
}

void DrawTriangle::UninitScene()
{
    SAFE_RELEASE(m_pVertexBuffer);
    SAFE_RELEASE(m_pInputLayout);
    SAFE_RELEASE(m_pVertexShader);
    SAFE_RELEASE(m_pPixelShader);
}
