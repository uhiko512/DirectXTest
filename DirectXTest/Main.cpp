// DirectXTest.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Main.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	Win32App::Init(hInstance, nCmdShow);
	auto renderer = make_shared<Renderer>();
	Win32App::SetRenderer(renderer);
	auto scene = make_shared<Scene>(renderer);
	Win32App::SetScene(scene);

    return Win32App::Run();
}
