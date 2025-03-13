#pragma once
#include "GameEvents/GameEvent.h"
#include "GameEvents/OnResizeGameEventArguments.h"

#include <string>
#include <Windows.h>

class MasterWindow
{
private:
	static bool m_IsMainInstanceActive;
public:
	static GameEvent<OnResizeGameEventArguments> OnResizeEvent;
	//static ScreenState ScreenState; // TODO: This is techinically not needed and could just reuse the camera screen state

	HWND HWnd;
	LPCWSTR const ClassName = L"Master Window";

	MasterWindow();

	static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool RunMessageLoop();

	void Register(HINSTANCE* phInst);

	static void LogToConsole(std::wstring s);
};