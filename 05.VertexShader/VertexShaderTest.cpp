#include "VertexShaderTest.h"
#include <directxtk/simplemath.h>
#include <d3dcompiler.h>
#include "../Main/Helper.h"
#pragma comment (lib, "d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

using namespace DirectX::SimpleMath;
struct Vertex
{
    Vector3 position;		// ���� ��ġ ����.
    Vector4 color;			// ���� ���� ����.

    Vertex(float x, float y, float z) : position(x, y, z) { }
    Vertex(Vector3 position) : position(position) { }

    Vertex(Vector3 position, Vector4 color)
        : position(position), color(color) { }
};

struct CB_Transform
{
    Matrix mWorld;
    Matrix mView;
    Matrix mProjection;
};

VertexShaderTest::VertexShaderTest(HINSTANCE hInstance)
    :MainApp(hInstance)
{
}

VertexShaderTest::~VertexShaderTest()
{
    UninitScene();
    UninitD3D();
}

bool VertexShaderTest::Initialize(UINT Width, UINT Height)
{
    MainApp::Initialize(Width, Height);
    if (!InitD3D())
        return false;

    if (!InitScene())
        return false;
    return true;
}
void VertexShaderTest::Update()
{
    __super::Update();
    m_World = DirectX::XMMatrixRotationY(GameTimer::m_Instance->TotalTime());
}
void VertexShaderTest::Render()
{
    float color[4] = { 0.0f, 0.5f, 0.5f, 1.0f };

    // ȭ�� ĥ�ϱ�.
    m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);

    ///ã�ƺ���
    // Update variables
    CB_Transform cb;
    cb.mWorld = XMMatrixTranspose(m_World);
    cb.mView = XMMatrixTranspose(m_View);
    cb.mProjection = XMMatrixTranspose(m_Projection);
    m_pDeviceContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &cb, 0, 0);

    // Draw�迭 �Լ��� ȣ���ϱ����� ������ ���������ο� �ʼ� �������� ������ �ؾ��Ѵ�.	
    m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // ������ �̾ �׸� ��� ����.
    m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_VertextBufferStride, &m_VertextBufferOffset);
    m_pDeviceContext->IASetInputLayout(m_pInputLayout);
    m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

    m_pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
    m_pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);
    m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);


    ///ã�ƺ���
    m_pDeviceContext->DrawIndexed(m_nIndices, 0, 0);

    // Render a triangle	
   // m_pDeviceContext->Draw(3, 0);

    // Present the information rendered to the back buffer to the front buffer (the screen)
    m_pSwapChain->Present(0, 0);

}

bool VertexShaderTest::Run()
{
    MainApp::Run();
    return true;
}



bool VertexShaderTest::InitD3D()
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

    //4. ����Ʈ ����.	
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

void VertexShaderTest::UninitD3D()
{
    SAFE_RELEASE(m_pDevice);
    SAFE_RELEASE(m_pDeviceContext);
    SAFE_RELEASE(m_pSwapChain);
    SAFE_RELEASE(m_pRenderTargetView);
}

// 1. Render() ���� ���������ο� ���ε��� ���ؽ� ���۹� ���� ���� �غ�
// 2. Render() ���� ���������ο� ���ε��� InputLayout ���� 	
// 3. Render() ���� ���������ο� ���ε���  ���ؽ� ���̴� ����
// 4. Render() ���� ���������ο� ���ε��� �ε��� ���� ����
// 5. Render() ���� ���������ο� ���ε��� �ȼ� ���̴� ����
bool VertexShaderTest::InitScene()
{
    using namespace DirectX::SimpleMath;
    HRESULT hr = 0; // �����.

    Vertex vertices[] =
    {
    	{ Vector3(-1.0f, 1.0f, -1.0f),	Vector4(1.0f, 0.0f, 0.0f, 1.0f) },
       { Vector3(1.0f, 1.0f, -1.0f),	Vector4(0.7f, 0.0f, 0.0f, 1.0f) },
       { Vector3(1.0f, 1.0f, 1.0f),	Vector4(0.4f, 0.0f, 0.0f, 1.0f) },
       { Vector3(-1.0f, 1.0f, 1.0f),	Vector4(0.7f, 0.0f, 0.0f, 1.0f) },
       { Vector3(-1.0f, -1.0f, -1.0f), Vector4(0.5f, 0.0f, 0.0f, 1.0f) },
       { Vector3(1.0f, -1.0f, -1.0f),	Vector4(0.4f, 0.0f, 0.0f, 1.0f) },
       { Vector3(1.0f, -1.0f, 1.0f),	Vector4(0.2f, 0.0f, 0.0f, 1.0f) },
       { Vector3(-1.0f, -1.0f, 1.0f),	Vector4(0.4f, 0.0f, 0.0f, 1.0f) },
    };

    D3D11_BUFFER_DESC BufferDesc = {};
    BufferDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(vertices);
    ///�̰� ����?
    BufferDesc.CPUAccessFlags = 0;
    //vbDesc.MiscFlags = 0;
    BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    BufferDesc.Usage = D3D11_USAGE_DEFAULT;

    // ���� ���� ����.
    D3D11_SUBRESOURCE_DATA vbData = {};
    vbData.pSysMem = vertices;
    HR_T(m_pDevice->CreateBuffer(&BufferDesc, &vbData, &m_pVertexBuffer));

    // ���ؽ� ���� ���� 
    m_VertextBufferStride = sizeof(Vertex);
    m_VertextBufferOffset = 0;


    ///�̰͵� ã�ƺ���
    // 2. Render() ���� ���������ο� ���ε��� InputLayout ���� 	
    D3D11_INPUT_ELEMENT_DESC layout[] = // �Է� ���̾ƿ�.
    {   // SemanticName , SemanticIndex , Format , InputSlot , AlignedByteOffset , InputSlotClass , InstanceDataStepRate	
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },// 4byte * 3 = 12byte
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 } // 12 ��� D3D11_APPEND_ALIGNED_ELEMENT ��� ����.
    };
    ID3DBlob* vertexShaderBuffer = nullptr;
    HR_T(CompileShaderFromFile(L"BasicVertexShader.hlsl", "main", "vs_4_0", &vertexShaderBuffer));

    HR_T(m_pDevice->CreateInputLayout(layout, ARRAYSIZE(layout), vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_pInputLayout));

    // 3. Render���� ���������ο� ���ε���  ���ؽ� ���̴� ����
    HR_T(m_pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_pVertexShader));

    SAFE_RELEASE(vertexShaderBuffer);


    // 4-1. Render���� ���������ο� ���ε��� �ȼ� ���̴� ����
    //ID3DBlob* pixelShaderBuffer = nullptr;
    //HR_T(CompileShaderFromFile(L"BasicPixelShader.hlsl", "main", "ps_4_0", &pixelShaderBuffer));
    //HR_T(m_pDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pPixelShader));
    //SAFE_RELEASE(pixelShaderBuffer);


    /// 4-2. Render() ���� ���������ο� ���ε��� �ε��� ���� ����
    UINT indices[] =
    {
        3,1,0, 2,1,3,
        0,5,4, 1,5,0,
        3,4,7, 0,4,3,
        1,6,5, 2,6,1,
        2,7,6, 3,7,2,
        6,4,5, 7,4,6,
    };
    m_nIndices = ARRAYSIZE(indices);	// �ε��� ���� ����.
    //D3D11_BUFFER_DESC ibDesc = {};
    BufferDesc.ByteWidth = sizeof(UINT) * ARRAYSIZE(indices);
    BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    BufferDesc.Usage = D3D11_USAGE_DEFAULT;
    D3D11_SUBRESOURCE_DATA ibData = {};
    ibData.pSysMem = indices;
    HR_T(m_pDevice->CreateBuffer(&BufferDesc, &ibData, &m_pIndexBuffer));

    // 5. Render() ���� ���������ο� ���ε��� �ȼ� ���̴� ����
    ID3D10Blob* pixelShaderBuffer = nullptr;
    HR_T(CompileShaderFromFile(L"BasicPixelShader.hlsl", "main", "ps_4_0", &pixelShaderBuffer));
    HR_T(m_pDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),
        pixelShaderBuffer->GetBufferSize(), NULL, &m_pPixelShader));
    SAFE_RELEASE(pixelShaderBuffer);	// �ȼ� ���̴� ���� ���̻� �ʿ����.


    // 6. Render() ���� ���������ο� ���ε��� ��� ���� ����
    // Create the constant buffer
    BufferDesc.Usage = D3D11_USAGE_DEFAULT;
    BufferDesc.ByteWidth = sizeof(CB_Transform);
    BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    BufferDesc.CPUAccessFlags = 0;
    HR_T(m_pDevice->CreateBuffer(&BufferDesc, nullptr, &m_pConstantBuffer));

    // ���̴��� ������ ������ ����
    // Initialize the world matrix
    m_World = DirectX::XMMatrixIdentity();

    // Initialize the view matrix
    DirectX::XMVECTOR Eye = DirectX::XMVectorSet(0.0f, 1.0f, -5.0f, 0.0f);
    DirectX::XMVECTOR At = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    DirectX::XMVECTOR Up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    m_View = DirectX::XMMatrixLookAtLH(Eye, At, Up);

    // Initialize the projection matrix
    m_Projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, m_ClientWidth / (FLOAT)m_ClientHeight, 0.01f, 100.0f);
    return true;
}







void VertexShaderTest::UninitScene()
{
    SAFE_RELEASE(m_pVertexBuffer);
    SAFE_RELEASE(m_pInputLayout);
    SAFE_RELEASE(m_pVertexShader);
    SAFE_RELEASE(m_pPixelShader);
    SAFE_RELEASE(m_pIndexBuffer);
    SAFE_RELEASE(m_pConstantBuffer);
}
