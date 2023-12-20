#include "DrawTriangle.h"
#include <directxtk/simplemath.h>
#include <d3dcompiler.h>
#include "../Main/Helper.h"
#pragma comment (lib, "d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

struct Vertex
{
	DirectX::SimpleMath::Vector3 position;		// ��ġ ����.
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

    // ȭ�� ĥ�ϱ�.
    m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);

    // Draw�迭 �Լ��� ȣ���ϱ����� ������ ���������ο� �ʼ� �������� ������ �ؾ��Ѵ�.	
    m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // ������ �̾ �׸� ��� ����.
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
    ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC)); //�̰� ����

    swapDesc.BufferCount = 1;
    //����� ������
    swapDesc.BufferDesc.Width = m_ClientWidth;
    swapDesc.BufferDesc.Height = m_ClientHeight;
    //ȭ�� �ֻ��� ���� //RefreshRate : ȭ�� ���ŷ�(������)
    swapDesc.BufferDesc.RefreshRate.Numerator = 60;    //����
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


    // 4. ����Ÿ�ٺ� ����.  (����۸� �̿��ϴ� ����Ÿ�ٺ�)	
    ID3D11Texture2D* pBackBuffer = nullptr;
    HR_T(m_pSwapChain->GetBuffer(0,
        __uuidof(ID3D11Texture2D),
        reinterpret_cast<void**>(&pBackBuffer)));


    HR_T(m_pDevice->CreateRenderTargetView(pBackBuffer,
        nullptr,
        &m_pRenderTargetView)); // �ؽ�ó�� ���� ���� ����
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
    HRESULT hr = 0; // �����.
    ID3D10Blob* errorMessage = nullptr;	 // ������ ���� �޽����� ����� ����.	

    //1. Render() ���� ���������ο� ���ε��� ���ؽ� ���۹� ���� ���� �غ�
    // ������ VertexShader�� World, View, Projection ��ȯ�� ������� �����Ƿ� 
    // ���� Normalized Device Coordinate(��ǥ��)�� ��ġ�� �����Ѵ�.
    //      /---------------------(1,1,1)   z���� ���̰�
    //     /                      / |   
    // (-1,1,0)----------------(1,1,0)        
    //   |         v1           |   |
    //   |        /   `         |   |       �߾��� (0,0,0)  
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

    // ���� ���� ����.
    D3D11_SUBRESOURCE_DATA vbData = {};
    vbData.pSysMem = vertices;
    HR_T(m_pDevice->CreateBuffer(&vbDesc, &vbData, &m_pVertexBuffer));

    // ���ؽ� ���� ���� 
    m_VertextBufferStride = sizeof(Vertex);
    m_VertextBufferOffset = 0;


    // 2. Render() ���� ���������ο� ���ε��� InputLayout ���� 	
    D3D11_INPUT_ELEMENT_DESC layout[] =  // ��ǲ ���̾ƿ��� ���ؽ� ���̴��� �Է¹��� �������� ������ �����Ѵ�.
    {// SemanticName , SemanticIndex , Format , InputSlot , AlignedByteOffset , InputSlotClass , InstanceDataStepRate		
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    ID3DBlob* vertexShaderBuffer = nullptr;
    HR_T(CompileShaderFromFile(L"BasicVertexShader.hlsl", "main", "vs_4_0", &vertexShaderBuffer));
    HR_T( m_pDevice->CreateInputLayout(layout, ARRAYSIZE(layout),vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_pInputLayout));


    // 3. Render���� ���������ο� ���ε���  ���ؽ� ���̴� ����
    HR_T(m_pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),vertexShaderBuffer->GetBufferSize(), NULL, &m_pVertexShader));
    SAFE_RELEASE(vertexShaderBuffer);


    // 4. Render���� ���������ο� ���ε��� �ȼ� ���̴� ����
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
