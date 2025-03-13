#include "HeaderFiles/Game/GameInstance.h"
#include "HeaderFiles/MasterWindow.h"

#include "KeyboardInterfaceAPI/Headers/KeyboardInterface/KeyboardState.h"

bool MasterWindow::m_IsMainInstanceActive = true;
//SandboxEngine::Render::ScreenState MasterWindow::ScreenState = SandboxEngine::Render::ScreenState();
GameEvent<OnResizeGameEventArguments> MasterWindow::OnResizeEvent;

MasterWindow::MasterWindow() { HWnd = 0; m_IsMainInstanceActive = true; }

LRESULT CALLBACK MasterWindow::WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (KeyboardInterface::KeyboardState::ProcessMessages(uMsg, wParam, lParam))
		return 0;
	switch (uMsg)
	{
	case WM_SIZE:
	{
		OnResizeEvent.Fire(OnResizeGameEventArguments(LOWORD(lParam), HIWORD(lParam)));
		SandboxEngine::Game::GameInstance::MainCamera.MainScreen.Resize(LOWORD(lParam), HIWORD(lParam));
		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);

		DestroyWindow(hwnd);
		UnregisterClass(L"Master Window", NULL);
		m_IsMainInstanceActive = false;
		break;
	}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

bool MasterWindow::RunMessageLoop()
{
	MSG msg;
	while (PeekMessage(&msg, HWnd, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return m_IsMainInstanceActive;
}

void MasterWindow::Register(HINSTANCE* phInst)
{
	WNDCLASS wndClass = {};
	wndClass.hInstance = *phInst;
	wndClass.lpszClassName = ClassName;
	wndClass.lpfnWndProc = WindowProcedure;

	RegisterClass(&wndClass);
}

void MasterWindow::LogToConsole(std::wstring s)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	WriteConsole(handle, s.c_str(), s.size(), nullptr, nullptr);
}