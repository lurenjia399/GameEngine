#include "WindowsMessageProcessing.h"
#include "../../Component/Input/Input.h"
#include <windowsx.h>

LRESULT CALLBACK EngineWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	case WM_RBUTTONDOWN:
		MouseDownDelegate.Broadcast(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), "R");
		return 0;
	case WM_RBUTTONUP:
		MouseUpDelegate.Broadcast(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), "R");
		return 0;
	case WM_LBUTTONDOWN:
		MouseDownDelegate.Broadcast(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), "L");
		return 0;
	case WM_LBUTTONUP:
		MouseUpDelegate.Broadcast(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), "L");
		return 0;
	case WM_MOUSEMOVE:
		MouseMoveDelegate.Broadcast(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), "RandL");
		return 0;
		
	}
	//交给windows自己处理完成
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
