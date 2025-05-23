#include "pch.h"
#include "MainApp.h"
#include <imgui_impl_win32.h>

MainApp* MainApp::m_pInstance = nullptr;
HWND MainApp::m_hWnd;

LRESULT CALLBACK DefaultWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return  MainApp::m_pInstance->WndProc(hWnd, message, wParam, lParam);
}
MainApp::MainApp(HINSTANCE hInstance)
	:m_hInstance(hInstance), m_szWindowClass(L"DefaultWindowCalss"), m_szTitle(L"GameApp"), m_ClientWidth(1920), m_ClientHeight(1080)
{
	MainApp::m_pInstance = this;
	m_wcex.hInstance = hInstance;
	
	m_wcex.cbSize = sizeof(WNDCLASSEX);
	m_wcex.style = CS_HREDRAW | CS_VREDRAW;
	m_wcex.lpfnWndProc = DefaultWndProc;
	m_wcex.cbClsExtra = 0;
	m_wcex.cbWndExtra = 0;
	m_wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	m_wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	m_wcex.lpszClassName = m_szWindowClass;
}

MainApp::~MainApp()
{
}


bool MainApp::Initialize(UINT Width, UINT Height)
{
	m_ClientWidth = Width;
	m_ClientHeight = Height;

	// 등록
	RegisterClassExW(&m_wcex);

	// 원하는 크기가 조정되어 리턴
	RECT rcClient = { 0, 0, (LONG)Width, (LONG)Height };
	AdjustWindowRect(&rcClient, WS_OVERLAPPEDWINDOW, FALSE);

	//생성
	m_hWnd = CreateWindowW(m_szWindowClass, m_szTitle, WS_OVERLAPPEDWINDOW,
		100, 100,	// 시작 위치
		rcClient.right - rcClient.left, rcClient.bottom - rcClient.top,
		nullptr, nullptr, m_hInstance, nullptr);

	if (!m_hWnd)
	{
		return false;
	}

	// 윈도우 보이기
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	m_currentTime = m_previousTime = (float)GetTickCount64() / 1000.0f;


	if (m_D3DRenderer.Initialize(m_hWnd, m_ClientWidth, m_ClientHeight))
	{
		return false;
	}

	return true;
}

bool MainApp::Run()
{
	// PeekMessage 메세지가 있으면 true,없으면 false
	while (TRUE)
	{
		if (PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE))
		{
			if (m_msg.message == WM_QUIT)
				break;

			//윈도우 메시지 처리 
			TranslateMessage(&m_msg); // 키입력관련 메시지 변환  WM_KEYDOWN -> WM_CHAR
			DispatchMessage(&m_msg);
		}
		else
		{
			Update();
			Render();
		}
	}
	return 0;
}

void MainApp::Update()
{
	m_Timer.Tick();
	m_D3DRenderer.Update();
}

void MainApp::Render()
{
	m_D3DRenderer.Render();
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MainApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
//extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
//LRESULT CALLBACK PBRTest::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
//		return true;
//	return __super::WndProc(hWnd, message, wParam, lParam);
//}