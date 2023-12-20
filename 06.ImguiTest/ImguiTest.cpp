#include "ImguiTest.h"
#include <directxtk/simplemath.h>
#include <d3dcompiler.h>
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
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



ImguiTest::ImguiTest(HINSTANCE hInstance)
    : MainApp(hInstance), m_firstCubeRotation{}, m_firstCubePos(), m_secondCubePos(5, 0, 0), m_thirdCubePos(2.5f, 0, 0),
    m_cubesScale(1.f, 1.f, 1.f), m_cameraTrans(0, 0, -50.f), m_cameraRot(), m��cameraFov(.5f), m_cameraDistance(1.f, 100.f),
    m_angle(0), m_plusangle(100.f)
{
}

ImguiTest::~ImguiTest()
{
    UninitScene();
    UninitImGUI();
    UninitD3D();
}

bool ImguiTest::Initialize(UINT Width, UINT Height)
{
    MainApp::Initialize(Width, Height);
    if (!InitD3D())
        return false;

    if (!InitScene())
        return false;
    if (!InitImGUI())
        return false;
    return true;
}
void ImguiTest::Update()
{
    using namespace DirectX;
    __super::Update();


    ///ť�� ���� ����.
    {
        m_angle += m_plusangle * GameTimer::m_Instance->DeltaTime();
        m_angle = fmod(m_angle, 360);
        const float cubeRotation = XMConvertToRadians(m_angle);
        ///1�� ť�� ����
        XMMATRIX scaleMatrix = DirectX::XMMatrixScaling(m_cubesScale.x, m_cubesScale.y, m_cubesScale.z);
        XMMATRIX rotationMatrix = XMMatrixRotationY(cubeRotation);
        XMMATRIX translationMatrix = XMMatrixTranslation(m_firstCubePos.x, m_firstCubePos.y, m_firstCubePos.z);
        m_transformData1.mWorld = XMMatrixTranspose(m_MainWorld * scaleMatrix * rotationMatrix * translationMatrix);
        m_transformData1.mView = XMMatrixTranspose(m_View);
        m_transformData1.mProjection = XMMatrixTranspose(m_Projection);

        ///2�� ť�� ����
        m_World2 = rotationMatrix * translationMatrix * static_cast<XMMATRIX>(m_MainWorld);
        rotationMatrix = XMMatrixRotationY(cubeRotation);
        translationMatrix = XMMatrixTranslation(m_secondCubePos.x, m_secondCubePos.y, m_secondCubePos.z);
        m_transformData2.mWorld = XMMatrixTranspose(scaleMatrix * rotationMatrix * translationMatrix * static_cast<XMMATRIX>(m_World2));
        m_transformData2.mView = XMMatrixTranspose(m_View);
        m_transformData2.mProjection = XMMatrixTranspose(m_Projection);

        ///3�� ť�� ����
        m_World3 = rotationMatrix * translationMatrix * static_cast<XMMATRIX>(m_World2);
        rotationMatrix = XMMatrixRotationY(cubeRotation);
        translationMatrix = XMMatrixTranslation(m_thirdCubePos.x, m_thirdCubePos.y, m_thirdCubePos.z);
        m_transformData3.mWorld = XMMatrixTranspose(scaleMatrix * rotationMatrix * translationMatrix * static_cast<XMMATRIX>(m_World3));
        m_transformData3.mView = XMMatrixTranspose(m_View);
        m_transformData3.mProjection = XMMatrixTranspose(m_Projection);
    }


    ///ī�޶� ����!
    {
        float pitch = XMConvertToRadians(m_cameraRot.x);
        float yaw = XMConvertToRadians(m_cameraRot.y);
        float roll = XMConvertToRadians(m_cameraRot.z);
        XMMATRIX cameraRotMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

        // ī�޶� ��ġ(eye)�� ȸ���� �������� ��ȯ
        XMVECTOR eye = XMVectorSet(m_cameraTrans.x, m_cameraTrans.y, m_cameraTrans.z, 0.f);
        // ī�޶��� ���� ���͸� ����
        XMVECTOR forward = XMVector3TransformCoord(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), cameraRotMatrix);
        // ī�޶��� ���� ���͸� ����
        XMVECTOR up = XMVector3TransformCoord(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), cameraRotMatrix);
        // �� ��Ʈ���� ����
        m_View = XMMatrixLookToLH(eye, forward, up);

        ///ī�޶� ����ó��
        if (m_cameraDistance.x >= m_cameraDistance.y)
            m_cameraDistance.y = m_cameraDistance.x + 1.f;
        if (m_cameraDistance.x <= 0)
            m_cameraDistance.x = 0.1f;
        if (m��cameraFov <= 0)
            m_cameraDistance.x = 0.001f;
    }

    ///���� ����.
    m_Projection = XMMatrixPerspectiveFovLH(m��cameraFov, (float)GetClientWidth() / GetClientHeight(), m_cameraDistance.x, m_cameraDistance.y);

}
void ImguiTest::Render()
{
    float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    //this->m_pDeviceContext->OMSetRenderTargets(1, &this->m_pRenderTargetView, nullptr);
    // ȭ�� ĥ�ϱ�.
    m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);
    m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0); // �������� 1.0f�� �ʱ�ȭ.



    /// Draw�迭 �Լ��� ȣ���ϱ����� ������ ���������ο� �ʼ� �������� ������ �ؾ��Ѵ�.	
    {
        m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // ������ �̾ �׸� ��� ����.
        m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_VertextBufferStride, &m_VertextBufferOffset);
        m_pDeviceContext->IASetInputLayout(m_pInputLayout);
        m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
        m_pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
        m_pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);
        m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
    }

    ///ť�� �׸���.
    {
        m_pDeviceContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &m_transformData1, 0, 0);
        m_pDeviceContext->DrawIndexed(m_nIndices, 0, 0);

        m_pDeviceContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &m_transformData2, 0, 0);
        m_pDeviceContext->DrawIndexed(m_nIndices, 0, 0);

        m_pDeviceContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &m_transformData3, 0, 0);
        m_pDeviceContext->DrawIndexed(m_nIndices, 0, 0);
    }


#pragma region Imgui

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    /// Start the Dear ImGui frame
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }
    /// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static float a[4];
        ImGui::Begin("Testing Imgui");                          // Create a window called "Hello, world!" and append into it.
        if (ImGui::CollapsingHeader("Camera Setting"))
        {
            ImGui::Text("Camera Location:");
            ImGui::SameLine(155);
            ImGui::SliderFloat3("##SliderLocation", reinterpret_cast<float*>(&m_cameraTrans), -50.f, 50.f);

            ImGui::Text("Camera Rotation:");
            ImGui::SameLine(155);
            ImGui::SliderFloat3("##SliderRotation", reinterpret_cast<float*>(&m_cameraRot), -360.f, 360.f);

            ImGui::Text("Camera Fov:");
            ImGui::SameLine(155);
            ImGui::SliderFloat("##SliderFov", &m��cameraFov, 0.001f, 1.f);
            ImGui::Text("");

            ImGui::Text("Camera Distance:");
            ImGui::SameLine(155);
            ImGui::SliderFloat2("##SliderDistance", reinterpret_cast<float*>(&m_cameraDistance), 0.1f, 100.f);
            ImGui::Text("");
        }

        if (ImGui::CollapsingHeader("Cube Position Setting"))
        {
            ImGui::Text("First Cube Position:");
            ImGui::SameLine(155);
            ImGui::SliderFloat3("##SliderFirst", reinterpret_cast<float*>(&m_firstCubePos), -20.f, 20.f);
            ImGui::Text("");    ImGui::SameLine(155);
            ImGui::InputFloat3("##InputFirst", reinterpret_cast<float*>(&m_firstCubePos));
            ImGui::Text("");

            ImGui::Text("Second Cube Position:");
            ImGui::SameLine(155);
            ImGui::SliderFloat3("##SliderSecond", reinterpret_cast<float*>(&m_secondCubePos), -20.f, 20.f);
            ImGui::Text("");    ImGui::SameLine(155);
            ImGui::InputFloat3("##InputSecond", reinterpret_cast<float*>(&m_secondCubePos));
            ImGui::Text("");

            ImGui::Text("Third Cube Position:");
            ImGui::SameLine(155);
            ImGui::SliderFloat3("##ThirdCube", reinterpret_cast<float*>(&m_thirdCubePos), -20.f, 20.f);
            ImGui::Text("");    ImGui::SameLine(155);
            ImGui::InputFloat3("##InputThird", reinterpret_cast<float*>(&m_thirdCubePos));
            ImGui::Text("");

            ImGui::Text("Cubes Scale:");
            ImGui::SameLine(155);
            ImGui::SliderFloat3("##SliderCubeScale", reinterpret_cast<float*>(&m_cubesScale), 0.f, 2.f);
            ImGui::Text("");    ImGui::SameLine(155);
            ImGui::InputFloat3("##InputCubeScale", reinterpret_cast<float*>(&m_cubesScale));
            ImGui::Text("");

            ImGui::Text("Rotation Speed");
            ImGui::SameLine(155);
            ImGui::SliderFloat("##RotationSpeed", &m_plusangle, -720.f, 720.f);
            ImGui::Text("");    ImGui::SameLine(155);
            ImGui::InputFloat("##InputRotationspeed", &m_plusangle);
        }
        ImGui::SameLine();
        ImGui::End();
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }
#pragma endregion
    // Present the information rendered to the back buffer to the front buffer (the screen)
    m_pSwapChain->Present(0, 0);

}

bool ImguiTest::Run()
{
    MainApp::Run();
    return true;
}


#pragma region Init

bool ImguiTest::InitD3D()
{
    HRESULT hr = 0;

    // DirectX 11.0 systems
    DXGI_SWAP_CHAIN_DESC swapDesc = {};
    ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
    swapDesc.BufferCount = 1;
    swapDesc.BufferDesc.Width = m_ClientWidth;          //����� ������
    swapDesc.BufferDesc.Height = m_ClientHeight;        //����� ������
    swapDesc.BufferDesc.RefreshRate.Numerator = 60;     //ȭ�� �ֻ��� ���� //RefreshRate : ȭ�� ���ŷ�(������)
    swapDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    //UNORM : Unsigned Normalize (����ȭ)
    swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapDesc.OutputWindow = m_hWnd;
    swapDesc.SampleDesc.Count = true;  //MSAA(Multi Sampling Anti Aliasing)����
    swapDesc.SampleDesc.Quality = 0;
    swapDesc.Windowed = TRUE;;  //â��徵���� TRUE -> â���
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

    UINT creationFlags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#ifdef _DEBUG
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    HR_T(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlags, NULL, NULL, D3D11_SDK_VERSION, &swapDesc, &m_pSwapChain, &m_pDevice, NULL, &m_pDeviceContext));

    /// 4. ����Ÿ�ٺ� ����.  (����۸� �̿��ϴ� ����Ÿ�ٺ�)	
    {
        ID3D11Texture2D* pBackBuffer = nullptr;
        HR_T(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer)));
        HR_T(m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView)); // �ؽ�ó�� ���� ���� ����
        SAFE_RELEASE(pBackBuffer);
        m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);
    }

    ///5. ����Ʈ ����.	
    {
        D3D11_VIEWPORT viewport = {};
        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.Width = (float)m_ClientWidth;
        viewport.Height = (float)m_ClientHeight;
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;
        m_pDeviceContext->RSSetViewports(1, &viewport);
    }

    ///6. �X��&���ٽ� �� ����
    {
        D3D11_TEXTURE2D_DESC descDepth = {};
        descDepth.Width = m_ClientWidth;
        descDepth.Height = m_ClientHeight;
        descDepth.MipLevels = 1;
        descDepth.ArraySize = 1;
        descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        descDepth.SampleDesc.Count = 1;
        descDepth.SampleDesc.Quality = 0;
        descDepth.Usage = D3D11_USAGE_DEFAULT;
        descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        descDepth.CPUAccessFlags = 0;
        descDepth.MiscFlags = 0;
        ID3D11Texture2D* textureDepthStencil = nullptr;
        HR_T(m_pDevice->CreateTexture2D(&descDepth, nullptr, &textureDepthStencil));
        // Create the depth stencil view
        D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
        descDSV.Format = descDepth.Format;
        descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        descDSV.Texture2D.MipSlice = 0;
        HR_T(m_pDevice->CreateDepthStencilView(textureDepthStencil, &descDSV, &m_pDepthStencilView));
        SAFE_RELEASE(textureDepthStencil);
    }

    m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

    return true;

}


bool ImguiTest::InitImGUI()
{
    ///ImGui �ʱ�ȭ.

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();
    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(m_hWnd);
    ImGui_ImplDX11_Init(this->m_pDevice, this->m_pDeviceContext);
    return true;
}

bool ImguiTest::InitScene()
{
    using namespace DirectX::SimpleMath;

    HRESULT hr = 0; // �����.
    /// 1. Render() ���� ���������ο� ���ε��� ���ؽ� ���۹� ���� ���� �غ�		
    {
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
        BufferDesc.CPUAccessFlags = 0;
        BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        BufferDesc.Usage = D3D11_USAGE_DEFAULT;

        // ���� ���� ����.
        D3D11_SUBRESOURCE_DATA vbData = {};
        vbData.pSysMem = vertices;
        HR_T(m_pDevice->CreateBuffer(&BufferDesc, &vbData, &m_pVertexBuffer));

        // ���ؽ� ���� ���� 
        m_VertextBufferStride = sizeof(Vertex);
        m_VertextBufferOffset = 0;
    }
    /// 2. Render() ���� ���������ο� ���ε��� InputLayout ����
    ID3DBlob* vertexShaderBuffer = nullptr;
    {
        D3D11_INPUT_ELEMENT_DESC layout[] = // �Է� ���̾ƿ�.
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },// 4byte * 3 = 12byte
            { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 } // 12 ��� D3D11_APPEND_ALIGNED_ELEMENT ��� ����.
        };
        HR_T(CompileShaderFromFile(L"BasicVertexShader.hlsl", "main", "vs_4_0", &vertexShaderBuffer));

        HR_T(m_pDevice->CreateInputLayout(layout, ARRAYSIZE(layout), vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_pInputLayout));

    }
    /// 3. Render���� ���������ο� ���ε���  ���ؽ� ���̴� ����
    {
        HR_T(m_pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_pVertexShader));

    }
    SAFE_RELEASE(vertexShaderBuffer);
    /// 4. Render() ���� ���������ο� ���ε��� �ε��� ���� ����
    {
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

        D3D11_BUFFER_DESC BufferDesc = {};
        BufferDesc.ByteWidth = sizeof(UINT) * ARRAYSIZE(indices);
        BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        BufferDesc.Usage = D3D11_USAGE_DEFAULT;
        D3D11_SUBRESOURCE_DATA ibData = {};
        ibData.pSysMem = indices;
        HR_T(m_pDevice->CreateBuffer(&BufferDesc, &ibData, &m_pIndexBuffer));
    }
    /// 5. Render() ���� ���������ο� ���ε��� �ȼ� ���̴� ����
    {
        ID3D10Blob* pixelShaderBuffer = nullptr;
        HR_T(CompileShaderFromFile(L"BasicPixelShader.hlsl", "main", "ps_4_0", &pixelShaderBuffer));
        HR_T(m_pDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),
            pixelShaderBuffer->GetBufferSize(), NULL, &m_pPixelShader));
        SAFE_RELEASE(pixelShaderBuffer);	// �ȼ� ���̴� ���� ���̻� �ʿ����.
    }
    /// 6. Render() ���� ���������ο� ���ε��� ��� ���� ����
    {
        D3D11_BUFFER_DESC BufferDesc = {};
        BufferDesc.Usage = D3D11_USAGE_DEFAULT;
        BufferDesc.ByteWidth = sizeof(CB_Transform);
        BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        BufferDesc.CPUAccessFlags = 0;
        HR_T(m_pDevice->CreateBuffer(&BufferDesc, nullptr, &m_pConstantBuffer));
    }

    // Initialize the world matrix
    {
        m_MainWorld = DirectX::XMMatrixIdentity();
        m_World2 = DirectX::XMMatrixIdentity();
        m_World3 = DirectX::XMMatrixIdentity();
    }
    // Initialize the view matrix
    {
        DirectX::XMVECTOR Eye = DirectX::XMVectorSet(0.0f, 1.0f, -5.0f, 0.0f);
        DirectX::XMVECTOR At = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
        DirectX::XMVECTOR Up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
        m_View = DirectX::XMMatrixLookAtLH(Eye, At, Up);
        // Initialize the projection matrix
        m_Projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, m_ClientWidth / (FLOAT)m_ClientHeight, 0.01f, 100.0f);
    }
    return true;
}
#pragma endregion


#pragma region Uninit
void ImguiTest::UninitD3D()
{
    SAFE_RELEASE(m_pDevice);
    SAFE_RELEASE(m_pDeviceContext);
    SAFE_RELEASE(m_pSwapChain);
    SAFE_RELEASE(m_pRenderTargetView);
}
void ImguiTest::UninitImGUI()
{
    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}
void ImguiTest::UninitScene()
{
    SAFE_RELEASE(m_pVertexBuffer);
    SAFE_RELEASE(m_pInputLayout);
    SAFE_RELEASE(m_pVertexShader);
    SAFE_RELEASE(m_pPixelShader);
    SAFE_RELEASE(m_pIndexBuffer);
    SAFE_RELEASE(m_pConstantBuffer);
    SAFE_RELEASE(m_pDepthStencilView);
}
#pragma endregion

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ImguiTest::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;
    return __super::WndProc(hWnd, message, wParam, lParam);
}
