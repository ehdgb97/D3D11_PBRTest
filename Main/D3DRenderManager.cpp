#include "pch.h"
#include "D3DRenderManager.h"
 
#include "Actor.h"
#include "Helper.h"
#include <d3dcompiler.h>
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <psapi.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "DebugDraw.h"
#pragma comment (lib, "d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

using namespace DirectX::SimpleMath;
D3DRenderManager* D3DRenderManager::Instance = nullptr;

ID3D11Device* D3DRenderManager::m_pDevice = nullptr;

D3DRenderManager::D3DRenderManager()
	//: MainApp(hInstance)
	:
	 m_cameraTrans(0, 0, -1000.f)
	, m＿cameraFov(XM_PIDIV4)
	, m_cameraDistance(1.f, 3000.f)

	, m_meshRotationAngle(0.f, 30.f, 0.f)
	, m_meshRotationUse(true)
{
	Instance = this;
}

D3DRenderManager::~D3DRenderManager()
{
	UninitScene();
	UninitImGUI();
	UninitD3D();
}

bool D3DRenderManager::Initialize(HWND Handle,UINT Width, UINT Height)
{
	m_hWnd = Handle;
	
	if (!InitD3D(Width, Height))
		return false;
	if (!InitScene())
		return false;
	if (!InitImGUI())
		return false;
	return true;
}
void D3DRenderManager::Update()
{
	using namespace DirectX;
	//__super::Update();

	///카메라 셋팅!
	{
		float pitch = XMConvertToRadians(m_cameraRot.x);
		float yaw = XMConvertToRadians(m_cameraRot.y);
		float roll = XMConvertToRadians(m_cameraRot.z);
		XMMATRIX cameraRotMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

		// 카메라 위치(eye)와 회전을 라디안으로 변환
		XMVECTOR eye = XMVectorSet(m_cameraTrans.x, m_cameraTrans.y, m_cameraTrans.z, 0.f);
		// 카메라의 전방 벡터를 얻어옴
		XMVECTOR forward = XMVector3TransformCoord(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), cameraRotMatrix);
		// 카메라의 위쪽 벡터를 얻어옴
		XMVECTOR up = XMVector3TransformCoord(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), cameraRotMatrix);
		// 뷰 매트릭스 생성
		m_View = XMMatrixLookToLH(eye, forward, up);
		
		///카메라 예외처리
		if (m_cameraDistance.x >= m_cameraDistance.y)
			m_cameraDistance.y = m_cameraDistance.x + 1.f;
		if (m_cameraDistance.x <= 0)
			m_cameraDistance.x = 0.1f;
		if (m＿cameraFov <= 0)
			m＿cameraFov = 0.001f;
	}

	///최종 설정.
	m_Projection = XMMatrixPerspectiveFovLH(m＿cameraFov, m_viewport.Width / (FLOAT)m_viewport.Height, m_cameraDistance.x, m_cameraDistance.y);

	///라이트 계산
	{
		m_DirectionLightCB.Direction.Normalize();
		m_DirectionLightCB.EyePosition = m_cameraTrans;
	}
	///매쉬 셋팅 모음.
	{
		
		m_TransformCB.mView = XMMatrixTranspose(m_View);
		m_TransformCB.mProjection = XMMatrixTranspose(m_Projection);


		for (auto object : m_Actors)
		{
			object->SetWorld(m_World);
			object->Update(GameTimer::m_Instance->DeltaTime());
		}
	}


}
void D3DRenderManager::Render()
{

	using namespace DirectX;
	float color[4] = { .2f, .2f, .2f, 1.0f };
	//this->m_pDeviceContext->OMSetRenderTargets(1, &this->m_pRenderTargetView, nullptr);
	// 화면 칠하기.
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0); // 뎁스버퍼 1.0f로 초기화.

	// Draw계열 함수를 호출하기전에 렌더링 파이프라인에 필수 스테이지 설정을 해야한다.	
	{
		m_pDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 정점을 이어서 그릴 방식 설정.
		m_pDeviceContext->IASetInputLayout(m_pInputLayout);
		m_pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
		m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pTransformCB);
		m_pDeviceContext->VSSetConstantBuffers(1, 1, &m_pDirectionLightCB);
		m_pDeviceContext->VSSetConstantBuffers(2, 1, &m_pMaterialCB);
		m_pDeviceContext->VSSetConstantBuffers(3, 1, &m_pMatrixPalleteCB);
		m_pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);
		m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pTransformCB);
		m_pDeviceContext->PSSetConstantBuffers(1, 1, &m_pDirectionLightCB);
		m_pDeviceContext->PSSetConstantBuffers(2, 1, &m_pMaterialCB);
		m_pDeviceContext->PSSetSamplers(0, 1, &m_pSamplerLinear);
	}
	m_pDeviceContext->UpdateSubresource(m_pDirectionLightCB, 0, nullptr, &m_DirectionLightCB, 0, 0);
	

	for (auto object : m_Actors)
	{
		object->SetWorld(m_World);
		object->Render();
	}

	for (auto object : m_Actors)
	{
		object->DebugRender();
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
		{
			ImGui::Begin("Testing Imgui");                          // Create a window called "Hello, world!" and append into it.
			SIZE_T totalMemory, usedMemory;
			GetProcessMemoryUsage(totalMemory, usedMemory);

			ImGui::Text("Memory Usage:");
			ImGui::Text("Total Memory: %.2f MB", static_cast<float>(totalMemory) / (1024 * 1024));
			ImGui::Text("Used Memory: %.2f MB", static_cast<float>(usedMemory) / (1024 * 1024));
			if (ImGui::CollapsingHeader("Camera Setting"))
			{
				ImGui::Text("Camera Location:");
				ImGui::SameLine(150);
				ImGui::DragFloat3("##SliderLocation", reinterpret_cast<float*>(&m_cameraTrans), 0.1f, -1000.f, 1000.f);

				ImGui::Text("Camera Rotation:");
				ImGui::SameLine(150);
				ImGui::DragFloat3("##SliderRotation", reinterpret_cast<float*>(&m_cameraRot), 0.1f, -360.f, 360.f);

				ImGui::Text("Camera Fov:");
				ImGui::SameLine(150);
				ImGui::DragFloat("##SliderFov", &m＿cameraFov, 0.001f, 0.001f, 1.f);
				ImGui::Text("");

				ImGui::Text("Camera Distance:");
				ImGui::SameLine(150);
				ImGui::DragFloat2("##SliderDistance", reinterpret_cast<float*>(&m_cameraDistance), 1.f, 0.1f, 10000.f);
				ImGui::Text("");

			}
			if (ImGui::CollapsingHeader("Light Setting"))
			{
				ImGui::Text("Use BlinnPhong:");
				ImGui::SameLine(150);
				ImGui::Checkbox("##Check BlinnPhong", &m_DirectionLightCB.UseBlinnPhong);

				ImGui::Text("Light Direction:");
				ImGui::SameLine(150);
				ImGui::DragFloat3("##SliderLightDir", reinterpret_cast<float*>(&m_DirectionLightCB.Direction), 0.01f, -1.f, 1.f);

				ImGui::Text("Light Ambient:");
				ImGui::SameLine(150);
				ImGui::ColorEdit4("##EditDirectionAmbient", reinterpret_cast<float*>(&m_DirectionLightCB.Ambient));

				ImGui::Text("Light Diffuse:");
				ImGui::SameLine(150);
				ImGui::ColorEdit4("##EditDirectionDiffuse", reinterpret_cast<float*>(&m_DirectionLightCB.Diffuse));

				ImGui::Text("Light Specular:");
				ImGui::SameLine(150);
				ImGui::ColorEdit4("##EditDirectionSpecular", reinterpret_cast<float*>(&m_DirectionLightCB.Specular));




			}
			ImGui::SameLine();
			ImGui::End();

		}
		///Actor
		{
			ImGui::Begin("Actor Setting");                          // Create a window called "Hello, world!" and append into it.
			int i = 0;
			for (auto object : m_Actors)
			{
				string combinedMainstring;
				string combinedsubString = {};
				combinedMainstring = "GameObject" + std::to_string(i);
				if (ImGui::CollapsingHeader(combinedMainstring.c_str()))
				{
					Vector3 Scale = object->GetScale();
					Vector3 Pos = object->GetPos();
					Vector3 Angle = object->GetAngle();
					ImGui::Text("Object Position:");
					ImGui::SameLine(150);
					combinedsubString = "##GameObject" + std::to_string(i) + "Pos";
					ImGui::DragFloat3(combinedsubString.c_str(), reinterpret_cast<float*>(&Pos), 0.1f, -1000.f, 1000.f);

					ImGui::Text("Meshs Scale:");
					ImGui::SameLine(150);
					combinedsubString = "##GameObject" + std::to_string(i) + "Scale";
					ImGui::DragFloat(combinedsubString.c_str(), reinterpret_cast<float*>(&Scale), 0.05f, 0.1f, 100.f);
					ImGui::Text("");

					ImGui::Text("Use Rotation:");
					ImGui::SameLine(150);
					combinedsubString = "##GameObject" + std::to_string(i) + "Check";
					ImGui::Checkbox(combinedsubString.c_str(), &m_meshRotationUse);

					ImGui::Text("Set Angle");
					ImGui::SameLine(150);
					combinedsubString = "##GameObject" + std::to_string(i) + "Angle";
					ImGui::DragFloat3(combinedsubString.c_str(), reinterpret_cast<float*>(&Angle), 1, -360.f, 360.f);
					ImGui::Text("Rotation Speed");
					ImGui::SameLine(150);
					combinedsubString = "##GameObject" + std::to_string(i) + "RotationSpeed";
					ImGui::DragFloat3(combinedsubString.c_str(), reinterpret_cast<float*>(&m_meshRotationAngle), 1, -720.f, 720.f);
					Scale = { Scale.x,Scale.x,Scale.x };
					//if (m_meshRotationUse)
					Angle += m_meshRotationAngle * GameTimer::m_Instance->DeltaTime();
					///셋 pos
					object->SetPos(Pos);
					object->SetScale(Scale);
					object->SetAngle(Angle);
				}
				i++;
			}

			ImGui::SameLine();
			ImGui::End();
		}


		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
#pragma endregion
	m_pSwapChain->Present(0, 0);

}

bool D3DRenderManager::Run()
{
	return true;
}


#pragma region Init

bool D3DRenderManager::InitD3D(UINT ClientWidth, UINT ClientHeight)
{
	HRESULT hr = 0;

	// DirectX 11.0 systems
	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	swapDesc.BufferCount = 1;
	swapDesc.BufferDesc.Width = ClientWidth;          //백버퍼 사이즈
	swapDesc.BufferDesc.Height = ClientHeight;        //백버퍼 사이즈
	swapDesc.BufferDesc.RefreshRate.Numerator = 165;     //화면 주사율 설정 //RefreshRate : 화면 갱신률(프레임)
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    //UNORM : Unsigned Normalize (정규화)
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = m_hWnd;
	swapDesc.SampleDesc.Count = true;  //MSAA(Multi Sampling Anti Aliasing)쓸지
	swapDesc.SampleDesc.Quality = 0;
	swapDesc.Windowed = TRUE;;  //창모드쓸꺼냐 TRUE -> 창모드

	UINT creationFlags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HR_T(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlags, NULL, NULL, D3D11_SDK_VERSION, &swapDesc, &m_pSwapChain, &m_pDevice, NULL, &m_pDeviceContext));

	/// 4. 렌더타겟뷰 생성.  (백버퍼를 이용하는 렌더타겟뷰)	
	{
		ID3D11Texture2D* pBackBuffer = nullptr;
		HR_T(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer)));
		HR_T(m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView)); // 텍스처는 내부 참조 증가
		SAFE_RELEASE(pBackBuffer);
		m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);
	}

	///5. 뷰포트 설정.	
	{
		m_viewport.TopLeftX = 0;
		m_viewport.TopLeftY = 0;
		m_viewport.Width = (float)ClientWidth;
		m_viewport.Height = (float)ClientHeight;
		m_viewport.MinDepth = 0.0f;
		m_viewport.MaxDepth = 1.0f;
		m_pDeviceContext->RSSetViewports(1, &m_viewport);
	}

	///6. 뎊스&스텐실 뷰 생성
	{
		D3D11_TEXTURE2D_DESC descDepth = {};
		descDepth.Width =(UINT) m_viewport.Width;
		descDepth.Height = (UINT)m_viewport.Height;
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
	//7. 알파블렌딩을 위한 블렌드 상태 생성
	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	D3D11_RENDER_TARGET_BLEND_DESC rtBlendDesc = {};
	rtBlendDesc.BlendEnable = true; // 블렌드 사용 여부
	// FinalRGB = SrcRGB *SrcBlend + DestRGB*DestBlend
	rtBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
	rtBlendDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;	// SrcBlend는 SrcColor의 알파값
	rtBlendDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;	// DestBlend는 (1-SourceColor.a)
	// FinalAlpha = (SrcAlpha * SrcBlendAlpha) + (DestAlpha * DestBlendAlpha)
	rtBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rtBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;	// SrcBlendAlpha = 1
	rtBlendDesc.DestBlendAlpha = D3D11_BLEND_ONE;	// DestBlendAlpha = 1	
	rtBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; // 렌더타겟에 RGBA 모두 Write
	blendDesc.RenderTarget[0] = rtBlendDesc;
	HR_T(m_pDevice->CreateBlendState(&blendDesc, &m_pAlphaBlendState));
	return true;

}


bool D3DRenderManager::InitImGUI()
{
	///ImGui 초기화.

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

bool D3DRenderManager::InitScene()
{
	using namespace DirectX::SimpleMath;

	HRESULT hr = 0; // 결과값.
	/// 1. Render() 에서 파이프라인에 바인딩할 버텍스 버퍼및 버퍼 정보 준비		
	{

	}
	/// 2. Render() 에서 파이프라인에 바인딩할 InputLayout 생성
	ID3DBlob* vertexShaderBuffer = nullptr;
	{
		HR_T(CompileShaderFromFile(L"PBRTest_VS.hlsl", "main", "vs_5_0", &vertexShaderBuffer));
		D3D11_INPUT_ELEMENT_DESC layout[] = // 입력 레이아웃.
		{
			{ "VS_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT,  D3D11_INPUT_PER_VERTEX_DATA, 0 },// 4byte * 3 = 12byte
			{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // 12 대신 D3D11_APPEND_ALIGNED_ELEMENT 사용 가능.
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDINDICES", 0,  DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		HR_T(m_pDevice->CreateInputLayout(layout, ARRAYSIZE(layout), vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_pInputLayout));

	}
	/// 3. Render에서 파이프라인에 바인딩할  버텍스 셰이더 생성
	{
		HR_T(m_pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_pVertexShader));
	}
	SAFE_RELEASE(vertexShaderBuffer);

	/// 5. Render() 에서 파이프라인에 바인딩할 픽셀 셰이더 생성
	{
		ID3D10Blob* pixelShaderBuffer = nullptr;
		HR_T(CompileShaderFromFile(L"PBRTest_PS.hlsl", "main", "ps_5_0", &pixelShaderBuffer));
		HR_T(m_pDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),
			pixelShaderBuffer->GetBufferSize(), NULL, &m_pPixelShader));
		SAFE_RELEASE(pixelShaderBuffer);	// 픽셀 셰이더 버퍼 더이상 필요없음.
	}
	/// 6. Render() 에서 파이프라인에 바인딩할 상수 버퍼 생성
	{
		D3D11_BUFFER_DESC BufferDesc = {};
		BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		BufferDesc.ByteWidth = sizeof(CB_Transform);
		BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		BufferDesc.CPUAccessFlags = 0;
		HR_T(m_pDevice->CreateBuffer(&BufferDesc, nullptr, &m_pTransformCB));
		// Load the Texture

		BufferDesc = {  };
		BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		BufferDesc.ByteWidth = sizeof(CB_DirectionLight);
		BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		BufferDesc.CPUAccessFlags = 0;
		HR_T(m_pDevice->CreateBuffer(&BufferDesc, nullptr, &m_pDirectionLightCB));

		BufferDesc = {  };
		BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		BufferDesc.ByteWidth = sizeof(CB_Material);
		BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		BufferDesc.CPUAccessFlags = 0;
		HR_T(m_pDevice->CreateBuffer(&BufferDesc, nullptr, &m_pMaterialCB));

		BufferDesc = {  };
		BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		BufferDesc.ByteWidth = sizeof(CB_MatrixPallete);
		BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		BufferDesc.CPUAccessFlags = 0;
		HR_T(m_pDevice->CreateBuffer(&BufferDesc, nullptr, &m_pMatrixPalleteCB));

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
		m_World = DirectX::XMMatrixIdentity();
	}
	// Initialize the view matrix
	{
		DirectX::XMVECTOR Eye = DirectX::XMVectorSet(0.0f, 1.0f, -5.0f, 0.0f);
		DirectX::XMVECTOR At = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		DirectX::XMVECTOR Up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		m_View = DirectX::XMMatrixLookAtLH(Eye, At, Up);
		// Initialize the projection matrix
		m_Projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, m_viewport.Width / (FLOAT)m_viewport.Height, 0.01f, 100.0f);

	}
	int SpawnObject =4;
	DebugDraw::Initialize(m_pDevice, m_pDeviceContext);

	for (int i = 0; i < SpawnObject; i++)
	{
		shared_ptr<Actor> newObj = make_shared<Actor>();
		//newObj->SetFBX("cerberus.fbx");
		m_Actors.emplace_back(newObj);

	}
	return true;

}
#pragma endregion


#pragma region Uninit
void D3DRenderManager::UninitD3D()
{
	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pDevice);
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pRenderTargetView);
}
void D3DRenderManager::UninitImGUI()
{
	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void D3DRenderManager::GetProcessMemoryUsage(SIZE_T& total, SIZE_T& used)
{
	PROCESS_MEMORY_COUNTERS_EX pmc;
	if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) 
	{
		total = pmc.WorkingSetSize;
		used = pmc.PrivateUsage;
	}
	else 
	{
		// 에러 처리
	}
}

void D3DRenderManager::UninitScene()
{
	SAFE_RELEASE(m_pInputLayout);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pAlphaBlendState);
	SAFE_RELEASE(m_pSamplerLinear);
	SAFE_RELEASE(m_pTransformCB);
	SAFE_RELEASE(m_pDirectionLightCB);
	SAFE_RELEASE(m_pMaterialCB);
	SAFE_RELEASE(m_pMatrixPalleteCB);
}
#pragma endregion
