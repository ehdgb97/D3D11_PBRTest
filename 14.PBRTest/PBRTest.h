#pragma once
#include <memory>

#include "../Main/MainApp.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;


class PBRTest
	:public MainApp
{

public:
	PBRTest(HINSTANCE hInstance);
	~PBRTest();

	//virtual LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


	// MainApp을(를) 통해 상속됨
	virtual bool Initialize(UINT Width, UINT Height);
	virtual void Update();
	virtual void Render();

};

