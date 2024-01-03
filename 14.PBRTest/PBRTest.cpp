#include "pch.h"
#include "PBRTest.h"
#include "../Main/Helper.h"
#include <d3dcompiler.h>
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

using namespace DirectX::SimpleMath;


PBRTest::PBRTest(HINSTANCE hInstance)
	: MainApp(hInstance)
	
{
}

PBRTest::~PBRTest()
{
	
}

bool PBRTest::Initialize(UINT Width, UINT Height)
{
	__super::Initialize(Width, Height);

	return true;
	return true;
}

void PBRTest::Update()
{
	__super::Update();

}

void PBRTest::Render()
{
	__super::Render();
}
//extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
//LRESULT CALLBACK PBRTest::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
//		return true;
//	return __super::WndProc(hWnd, message, wParam, lParam);
//}