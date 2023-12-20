#include "TextureTest.h"
#include "../Main/Helper.h"
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <d3dcompiler.h>
#include <Directxtk/DDSTextureLoader.h>
#pragma comment (lib, "d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

using namespace DirectX::SimpleMath;
struct Vertex
{
    Vector3 position;		// ���� ��ġ ����.
    Vector3 Normal;
    Vector2 Texcode;


    Vertex(float x, float y, float z) : position(x, y, z) { }
    Vertex(Vector3 position) : position(position) { }

    Vertex(Vector3 position, Vector3 Normal, Vector2 Tex)
        : position(position), Normal(Normal), Texcode(Tex) { }

};



TextureTest::TextureTest(HINSTANCE hInstance)
    : MainApp(hInstance), m_firstCubeRotation{}, m_firstCubePos(), m_secondCubePos(5, 0, 0), m_thirdCubePos(2.5f, 0, 0),
    m_cubesScale(1.f, 1.f, 1.f), m_cameraTrans(0, 0, -50.f), m_cameraRot(), m��cameraFov(.5f), m_cameraDistance(1.f, 100.f),
    m_angle(0, 0, 0), m_plusangle(0, 0, 0)
{
}

TextureTest::~TextureTest()
{
    UninitScene();
    UninitImGUI();
    UninitD3D();
}

bool TextureTest::Initialize(UINT Width, UINT Height)
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
void TextureTest::Update()
{
    using namespace DirectX;
    __super::Update();
    ///����Ʈ ���
    {
        m_LightDirsEvaluated = m_InitialLightDirs;
    }

    ///ť�� ���� ����.
    {
        m_angle.x += m_plusangle.x * GameTimer::m_Instance->DeltaTime();
        m_angle.y += m_plusangle.y * GameTimer::m_Instance->DeltaTime();
        m_angle.z += m_plusangle.z * GameTimer::m_Instance->DeltaTime();
        m_angle.x = fmod(m_angle.x, 360);
        m_angle.y = fmod(m_angle.y, 360);
        m_angle.z = fmod(m_angle.z, 360);
        XMFLOAT3 cubeRotation;
        cubeRotation.x = XMConvertToRadians(m_angle.x);
        cubeRotation.y = XMConvertToRadians(m_angle.y);
        cubeRotation.z = XMConvertToRadians(m_angle.z);
        ///
        {
            ///1�� ť�� ����
            {
                Matrix scaleMatrix = XMMatrixScaling(m_cubesScale.x, m_cubesScale.y, m_cubesScale.z);
                Matrix rotationMatrix = XMMatrixRotationX(cubeRotation.x) * XMMatrixRotationY(cubeRotation.y) * XMMatrixRotationZ(cubeRotation.z);
                Matrix translationMatrix = XMMatrixTranslation(m_firstCubePos.x, m_firstCubePos.y, m_firstCubePos.z);
                Matrix localTransformMatrix = scaleMatrix * rotationMatrix * translationMatrix;

                m_World1 = localTransformMatrix * m_MainWorld;

                m_transformData1.mWorld = XMMatrixTranspose(m_World1);
                m_transformData1.mView = XMMatrixTranspose(m_View);
                m_transformData1.mProjection = XMMatrixTranspose(m_Projection);
                m_transformData1.vLightColor = m_LightColor;
                m_transformData1.vLightDir = m_LightDirsEvaluated;
                m_transformData1.vOutputColor = XMFLOAT4(0, 0, 0, 0);
            }

            {
                Matrix scaleMatrix = XMMatrixScaling(m_cubesScale.x, m_cubesScale.y, m_cubesScale.z);
                Matrix rotationMatrix = XMMatrixRotationX(cubeRotation.x) * XMMatrixRotationY(cubeRotation.y) * XMMatrixRotationZ(cubeRotation.z);

                Matrix translationMatrix = XMMatrixTranslation(m_secondCubePos.x, m_secondCubePos.y, m_secondCubePos.z);
                Matrix localTransformMatrix = scaleMatrix * rotationMatrix * translationMatrix;
                m_World2 = localTransformMatrix * m_World1;
                ///2�� ť�� ����
                m_transformData2.mWorld = XMMatrixTranspose(m_World2);
                m_transformData2.mView = XMMatrixTranspose(m_View);
                m_transformData2.mProjection = XMMatrixTranspose(m_Projection);
                m_transformData2.vLightColor = m_LightColor;
                m_transformData2.vLightDir = m_LightDirsEvaluated;
                m_transformData2.vOutputColor = XMFLOAT4(0, 0, 0, 0);
            }

            ///3�� ť�� ����
            {
                Matrix scaleMatrix = XMMatrixScaling(m_cubesScale.x, m_cubesScale.y, m_cubesScale.z);
                Matrix rotationMatrix = XMMatrixRotationX(cubeRotation.x) * XMMatrixRotationY(cubeRotation.y) * XMMatrixRotationZ(cubeRotation.z);

                Matrix translationMatrix = XMMatrixTranslation(m_thirdCubePos.x, m_thirdCubePos.y, m_thirdCubePos.z);
                Matrix localTransformMatrix = scaleMatrix * rotationMatrix * translationMatrix;

                m_World3 = localTransformMatrix * m_World2;
                m_transformData3.mWorld = XMMatrixTranspose(m_World3);
                m_transformData3.mView = XMMatrixTranspose(m_View);
                m_transformData3.mProjection = XMMatrixTranspose(m_Projection);
                m_transformData3.vLightColor = m_LightColor;
                m_transformData3.vLightDir = m_LightDirsEvaluated;
                m_transformData3.vOutputColor = XMFLOAT4(0, 0, 0, 0);
            }
        }


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
void TextureTest::Render()
{
    using namespace DirectX;
    float color[4] = { .2f, .2f, .2f, 1.0f };
    //this->m_pDeviceContext->OMSetRenderTargets(1, &this->m_pRenderTargetView, nullptr);
    // ȭ�� ĥ�ϱ�.
    m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);
    m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0); // �������� 1.0f�� �ʱ�ȭ.



    // Draw�迭 �Լ��� ȣ���ϱ����� ������ ���������ο� �ʼ� �������� ������ �ؾ��Ѵ�.	
    {
        m_pDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // ������ �̾ �׸� ��� ����.
        m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_VertextBufferStride, &m_VertextBufferOffset);
        m_pDeviceContext->IASetInputLayout(m_pInputLayout);
        m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
        m_pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
        m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);

        m_pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);
        m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
        m_pDeviceContext->PSSetShaderResources(0, 1, &m_pTextureRV);
        m_pDeviceContext->PSSetSamplers(0, 1, &m_pSamplerLinear);

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
    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static float a[4];
        ImGui::Begin("Testing Imgui");                          // Create a window called "Hello, world!" and append into it.
        if (ImGui::CollapsingHeader("Camera Setting"))
        {
            ImGui::Text("Camera Location:");
            ImGui::SameLine(155);
            ImGui::DragFloat3("##SliderLocation", reinterpret_cast<float*>(&m_cameraTrans), 0.1f, -50.f, 50.f);

            ImGui::Text("Camera Rotation:");
            ImGui::SameLine(155);
            ImGui::DragFloat3("##SliderRotation", reinterpret_cast<float*>(&m_cameraRot), 0.1f, -360.f, 360.f);

            ImGui::Text("Camera Fov:");
            ImGui::SameLine(155);
            ImGui::DragFloat("##SliderFov", &m��cameraFov, 0.001f, 0.001f, 1.f);
            ImGui::Text("");

            ImGui::Text("Camera Distance:");
            ImGui::SameLine(155);
            ImGui::DragFloat2("##SliderDistance", reinterpret_cast<float*>(&m_cameraDistance), 0.1f, 0.1f, 100.f);
            ImGui::Text("");
        }

        if (ImGui::CollapsingHeader("Cube Position Setting"))
        {
            ImGui::Text("First Cube Position:");
            ImGui::SameLine(155);
            ImGui::DragFloat3("##SliderFirst", reinterpret_cast<float*>(&m_firstCubePos), 0.1f, -20.f, 20.f);

            ImGui::Text("Second Cube Position:");
            ImGui::SameLine(155);
            ImGui::DragFloat3("##SliderSecond", reinterpret_cast<float*>(&m_secondCubePos), 0.1f, -20.f, 20.f);

            ImGui::Text("Third Cube Position:");
            ImGui::SameLine(155);
            ImGui::DragFloat3("##ThirdCube", reinterpret_cast<float*>(&m_thirdCubePos), 0.1f, -20.f, 20.f);

            ImGui::Text("Cubes Scale:");
            ImGui::SameLine(155);
            ImGui::DragFloat3("##SliderCubeScale", reinterpret_cast<float*>(&m_cubesScale), 0.05f, 0.f, 2.f);
            ImGui::Text("");
            m_cubesScale = { m_cubesScale.x,m_cubesScale.x,m_cubesScale.x };

            ImGui::Text("Set Angle");
            ImGui::SameLine(155);
            ImGui::DragFloat3("##SliderAngle", reinterpret_cast<float*>(&m_angle), 1, -360.f, 360.f);
            ImGui::Text("");

            ImGui::Text("Rotation Speed");
            ImGui::SameLine(155);
            ImGui::DragFloat3("##RotationSpeed", reinterpret_cast<float*>(&m_plusangle), 1, -720.f, 720.f);
        }
        if (ImGui::CollapsingHeader("Light Setting"))
        {
            static bool column2Visible = false;

            ImGui::Text("Light Direction:");
            ImGui::SameLine(155);
            ImGui::DragFloat3("##SliderLightDir", reinterpret_cast<float*>(&m_InitialLightDirs), 0.001, -1.f, 1.f);


            if (ImGui::Button("Color Setting"))
                column2Visible = !column2Visible;
            if (column2Visible)
                ImGui::ColorPicker4("", reinterpret_cast<float*>(&m_LightColor));
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

bool TextureTest::Run()
{
    MainApp::Run();
    return true;
}


#pragma region Init

bool TextureTest::InitD3D()
{
    HRESULT hr = 0;

    // DirectX 11.0 systems
    DXGI_SWAP_CHAIN_DESC swapDesc = {};
    swapDesc.BufferCount = 1;
    swapDesc.BufferDesc.Width = m_ClientWidth;          //����� ������
    swapDesc.BufferDesc.Height = m_ClientHeight;        //����� ������
    swapDesc.BufferDesc.RefreshRate.Numerator = 165;     //ȭ�� �ֻ��� ���� //RefreshRate : ȭ�� ���ŷ�(������)
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


bool TextureTest::InitImGUI()
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

bool TextureTest::InitScene()
{
    using namespace DirectX::SimpleMath;

    HRESULT hr = 0; // �����.
    /// 1. Render() ���� ���������ο� ���ε��� ���ؽ� ���۹� ���� ���� �غ�		
    {
        Vertex vertices[] =
        {
            { Vector3(-1.0f, 1.0f, -1.0f),	Vector3(0.0f, 1.0f, 0.0f),Vector2(0.0f, 0.0f) },// Normal Y +	 
            { Vector3(1.0f, 1.0f, -1.0f),	Vector3(0.0f, 1.0f, 0.0f),Vector2(1.0f, 0.0f)},
            { Vector3(1.0f, 1.0f, 1.0f),	Vector3(0.0f, 1.0f, 0.0f),Vector2(1.0f, 1.0f)},
            { Vector3(-1.0f, 1.0f, 1.0f),	Vector3(0.0f, 1.0f, 0.0f),Vector2(0.0f, 1.0f)},

            { Vector3(-1.0f, -1.0f, -1.0f), Vector3(0.0f, -1.0f, 0.0f),Vector2(0.0f, 0.0f)},// Normal Y -		
            { Vector3(1.0f, -1.0f, -1.0f),	Vector3(0.0f, -1.0f, 0.0f) ,Vector2(1.0f, 0.0f)},
            { Vector3(1.0f, -1.0f, 1.0f),	Vector3(0.0f, -1.0f, 0.0f) ,Vector2(1.0f, 1.0f) },
            { Vector3(-1.0f, -1.0f, 1.0f),	Vector3(0.0f, -1.0f, 0.0f) ,Vector2(0.0f, 1.0f)},

            { Vector3(-1.0f, -1.0f, 1.0f),	Vector3(-1.0f, 0.0f, 0.0f)  ,Vector2(0.0f, 1.0f) },//	Normal X -
            { Vector3(-1.0f, -1.0f, -1.0f), Vector3(-1.0f, 0.0f, 0.0f) ,Vector2(1.0f, 1.0f) },
            { Vector3(-1.0f, 1.0f, -1.0f),	Vector3(-1.0f, 0.0f, 0.0f)  ,Vector2(1.0f, 0.0f)    },
            { Vector3(-1.0f, 1.0f, 1.0f),	Vector3(-1.0f, 0.0f, 0.0f)  ,Vector2(0.0f, 0.0f)    },

            { Vector3(1.0f, -1.0f, 1.0f),	Vector3(1.0f, 0.0f, 0.0f),Vector2(1.0f, 1.0f) },// Normal X +
            { Vector3(1.0f, -1.0f, -1.0f),	Vector3(1.0f, 0.0f, 0.0f),Vector2(0.0f, 1.0f) },
            { Vector3(1.0f, 1.0f, -1.0f),	Vector3(1.0f, 0.0f, 0.0f),Vector2(0.0f, 0.0f) },
            { Vector3(1.0f, 1.0f, 1.0f),	Vector3(1.0f, 0.0f, 0.0f),Vector2(1.0f, 0.0f) },

            { Vector3(-1.0f, -1.0f, -1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector2(0.0f, 1.0f)  }, // Normal Z -
            { Vector3(1.0f, -1.0f, -1.0f),	Vector3(0.0f, 0.0f, -1.0f) , Vector2(1.0f, 1.0f)   },
            { Vector3(1.0f, 1.0f, -1.0f),	Vector3(0.0f, 0.0f, -1.0f) , Vector2(1.0f, 0.0f)   },
            { Vector3(-1.0f, 1.0f, -1.0f),	Vector3(0.0f, 0.0f, -1.0f) , Vector2(0.0f, 0.0f)  },

            //{ Vector3(-1.0f, -1.0f, 1.0f),	Vector3(0.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f) },// Normal Z +
            //{ Vector3(1.0f, -1.0f, 1.0f),	Vector3(0.0f, 0.0f, 1.0f), Vector2(1.0f, 0.0f) },
            //{ Vector3(1.0f, 1.0f, 1.0f),	Vector3(0.0f, 0.0f, 1.0f), Vector2(1.0f, 1.0f) },
            //{ Vector3(-1.0f, 1.0f, 1.0f),	Vector3(0.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f) },

        { Vector3(-1.0f, -1.0f, 1.0f)	,Vector3(0.0f, 0.0f, 1.0f), Vector2(1.0f, 1.0f) },
        { Vector3(1.0f, -1.0f, 1.0f)	,Vector3(0.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f) },
        { Vector3(1.0f, 1.0f, 1.0f)	,Vector3(0.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f) },
        { Vector3(-1.0f, 1.0f, 1.0f)	,Vector3(0.0f, 0.0f, 1.0f), Vector2(1.0f, 0.0f) },
        };
        /*for (auto v : vertices)
            v.normal.Normalize();*/

        D3D11_BUFFER_DESC BufferDesc = {};
        BufferDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(vertices);
        BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        BufferDesc.Usage = D3D11_USAGE_DEFAULT;
        BufferDesc.CPUAccessFlags = 0;

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
            { "VS_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT,  D3D11_INPUT_PER_VERTEX_DATA, 0 },// 4byte * 3 = 12byte
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 } // 12 ��� D3D11_APPEND_ALIGNED_ELEMENT ��� ����.
        };
        HR_T(CompileShaderFromFile(L"TextureTest_VS.hlsl", "main", "vs_4_0", &vertexShaderBuffer));

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
             3,1,0,      2,1,3,
             6,4,5,      7,4,6,
             11,9,8,     10,9,11,
             14,12,13,   15,12,14,
             19,17,16,   18,17,19,
        22,20,21, 23,20,22
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
        HR_T(CompileShaderFromFile(L"TextureTest_PS.hlsl", "main", "ps_4_0", &pixelShaderBuffer));
        HR_T(m_pDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),
            pixelShaderBuffer->GetBufferSize(), NULL, &m_pPixelShader));
        SAFE_RELEASE(pixelShaderBuffer);	// �ȼ� ���̴� ���� ���̻� �ʿ����.
    }
    /// 6. Render() ���� ���������ο� ���ε��� ��� ���� ����
    {
        D3D11_BUFFER_DESC BufferDesc = {};
        BufferDesc.Usage = D3D11_USAGE_DEFAULT;
        BufferDesc.ByteWidth = sizeof(ConstantBuffer);
        BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        BufferDesc.CPUAccessFlags = 0;
        HR_T(m_pDevice->CreateBuffer(&BufferDesc, nullptr, &m_pConstantBuffer));
        // Load the Texture
        HR_T(DirectX::CreateDDSTextureFromFile(m_pDevice, L"seafloor.dds", nullptr, &m_pTextureRV));
    }
    // Create the sample state
    D3D11_SAMPLER_DESC sampDesc = {};
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    HR_T(m_pDevice->CreateSamplerState(&sampDesc, &m_pSamplerLinear));




    // Initialize the world matrix
    {
        m_MainWorld = DirectX::XMMatrixIdentity();
        m_World1 = DirectX::XMMatrixIdentity();
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
void TextureTest::UninitD3D()
{
    SAFE_RELEASE(m_pDevice);
    SAFE_RELEASE(m_pDeviceContext);
    SAFE_RELEASE(m_pSwapChain);
    SAFE_RELEASE(m_pRenderTargetView);
}
void TextureTest::UninitImGUI()
{
    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}
void TextureTest::UninitScene()
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
LRESULT CALLBACK TextureTest::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;
    return __super::WndProc(hWnd, message, wParam, lParam);
}
