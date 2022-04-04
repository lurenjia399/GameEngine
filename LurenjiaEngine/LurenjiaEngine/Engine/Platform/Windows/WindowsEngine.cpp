#include "WindowsEngine.h"
#if defined(_WIN32)
#include "WindowsMessageProcessing.h"
int FWindowsEngine::PreInit(FWinMainCommandParameters InParameters)
{
	//处理命令

	if (InitWindows(InParameters))
	{

	}
	return 0;
}
int FWindowsEngine::Init()
{
	return 0;
}
int FWindowsEngine::PostInit()
{
	return 0;
}
void FWindowsEngine::Tick()
{
}
int FWindowsEngine::PreExit()
{
	return 0;
}
int FWindowsEngine::Exit()
{
	return 0;
}
int FWindowsEngine::PostExit()
{
	return 0;
}
bool FWindowsEngine::InitWindows(FWinMainCommandParameters InParameters)
{
	// 注册窗口
	WNDCLASSEX WindowsClass;
	WindowsClass.cbSize = sizeof(WNDCLASSEX);			//该对象实际占用多大内存
	WindowsClass.cbClsExtra = 0;						//是否需要额外空间
	WindowsClass.cbWndExtra = 0;						//是否需要额外内存
	WindowsClass.hbrBackground = nullptr;				//如果有设置就是GDI擦除
	WindowsClass.hCursor = LoadCursor(NULL, IDC_ARROW);	//设置一个箭头光标
	WindowsClass.hIcon = nullptr;						//应用程序放在磁盘上显示的图标
	WindowsClass.hIconSm = nullptr;						//应用程序显示在左上角的图标
	WindowsClass.hInstance = InParameters.HInstance;	//传递自己的窗口实例
	WindowsClass.lpszClassName = L"LurenjiaEngine";		//窗口名称
	WindowsClass.lpszMenuName = nullptr;				//设置菜单的名称
	WindowsClass.style = CS_VREDRAW | CS_HREDRAW;		//怎么绘制窗口，垂直或者水平
	WindowsClass.lpfnWndProc = EngineWindowProc;		//绑定消息处理函数

	//注册窗口
	ATOM RegisterAtom = RegisterClassEx(&WindowsClass);
	if (!RegisterAtom)
	{
		// 如果注册失败
		MessageBox(NULL, L"Register fail,", L"Error", MB_OK);
	}

	/*
	* {left, top, right, bottom}
	*/
	RECT Rect = { 0, 0, 1280, 720 };
	/*
	* 创建的视口
	* WS_OVERLAPPEDWINDOW 视口风格
	* 是否有菜单
	*/
	AdjustWindowRect(&Rect, WS_OVERLAPPEDWINDOW, NULL);

	int WindowWidth = Rect.right - Rect.left;
	int WindowHeight = Rect.bottom - Rect.top;

	HWND Hwnd = CreateWindowEx(
		NULL,						//窗口额外风格
		L"LurenjiaEngine",			//窗口名称
		L"Lurenjia Engine",			//窗口名称(显示在窗口的标题栏上)
		WS_OVERLAPPEDWINDOW,		//窗口风格
		100, 100,					//窗口左上角起始位置
		WindowWidth, WindowHeight,	//窗口长宽
		NULL,						//副窗口句柄
		nullptr,					//窗口菜单句柄
		InParameters.HInstance,		//窗口实例
		NULL						//可以往窗口中传参数
		);
	if (!Hwnd)
	{
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}
	
	//显示窗口
	ShowWindow(Hwnd, SW_SHOW);
	//窗口是脏的就刷一下
	UpdateWindow(Hwnd);
	//return false;
}
#endif
