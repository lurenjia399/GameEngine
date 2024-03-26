#include "WindowsEngine.h"
#include "../../Rendering/Engine/DirectX/Core/DirectXRenderingEngine.h"
#include "../../Config/EngineRenderConfig.h"
#include "../../Core/World.h"
#include "../../Core/Camera.h"
#include "../../Core/Viewport/Viewport.h"
#include "../Editor/EditorEngine.h"

#if defined(_WIN32)
#include "WindowsMessageProcessing.h"
CWindowsEngine::CWindowsEngine()
	: MyWindowsHandle(nullptr)
	, RenderingEngine({})
#if (EDITOR_ENGINE == 1) 
	, EditorEngine({})
#endif
	
{
	RenderingEngine = make_shared<CDirectXRenderingEngine>();
	RenderingEngine->ResetGuid("RenderingEngine");
#if (EDITOR_ENGINE == 1) 
	EditorEngine = make_shared<CEditorEngine>();
	EditorEngine->ResetGuid("EditorEngine");
#endif
}

CWindowsEngine::~CWindowsEngine()
{
	//delete RenderingEngine;
}

int CWindowsEngine::PreInit(FWinMainCommandParameters& InParameters)
{
	string LogPath = FEnginePathHelper::GetEngineLogsPath();

	init_log_system(LogPath.c_str());
	Engine_Log("Log Init.");

	//处理命令
	Engine_Log("Engine pre initialization complete.");

	//渲染引擎初始化
	RenderingEngine->PreInit(InParameters);

	return 0;
}

int CWindowsEngine::Init(FWinMainCommandParameters InParameters)
{
	PreInit(InParameters);

	InitWindows(InParameters);
	RenderingEngine->SetMainWindowsHandle(MyWindowsHandle);
	RenderingEngine->Init(InParameters);

	shared_ptr<CWorld> world = LurenjiaEngine::CreateObject<CWorld>(this, "World");
	RenderingEngine->SetWorld(world);
	Engine_Log("Engine initialization complete.");

	PostInit();
	return 0;
}

int CWindowsEngine::PostInit()
{
	Engine_Log("Engine post initialization complete.");

	RenderingEngine->PostInit();

	for (CCoreMinimalObject* Tmp : ObjectPool)
	{
		Tmp->BeginInit();
	}

	return 0;
}

void CWindowsEngine::Tick(float DeltaTime)
{
	for (CCoreMinimalObject* Tmp : ObjectPool)
	{
		if (Tmp->IsTick())
		{
			Tmp->Tick(DeltaTime);
		}
	}

	if (RenderingEngine->GetWorld() && RenderingEngine->GetWorld()->GetCamera())
	{
		shared_ptr<CWorld> world = RenderingEngine->GetWorld();
		FViewportInfo ViewportInfo = {};
		ViewportInfo.cameraPosition = XMFLOAT4(world->GetCamera()->GetPosition().x, world->GetCamera()->GetPosition().y, world->GetCamera()->GetPosition().z, 1.0f);
		ViewportInfo.ViewMatrix = world->GetCamera()->ViewMatrix;
		ViewportInfo.ProjectMatrix = world->GetCamera()->ProjectMatrix;
		RenderingEngine->UpdateConstantView(DeltaTime, ViewportInfo);
		RenderingEngine->Tick(DeltaTime);
	}
}

int CWindowsEngine::PreExit()
{
	RenderingEngine->PreExit();

	Engine_Log("Engine post exit complete.");
	return 1;
}

int CWindowsEngine::Exit()
{
	RenderingEngine->Exit();

	Engine_Log("Engine exit complete.");
	return 1;
}

int CWindowsEngine::PostExit()
{
	FEngineRenderConfig::Destroy();
	RenderingEngine->PostExit();


	Engine_Log("Engine post exit complete.");
	return 1;
}

bool CWindowsEngine::InitWindows(FWinMainCommandParameters InParameters)
{
	// 注册窗口
	WNDCLASSEX WindowsClass = {};
	WindowsClass.cbSize = sizeof(WNDCLASSEX);			//该对象实际占用多大内存
	WindowsClass.cbClsExtra = 0;						//是否需要额外空间
	WindowsClass.cbWndExtra = 0;						//是否需要额外内存
	WindowsClass.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);		//防止无聊的背景重绘
	WindowsClass.hCursor = LoadCursor(NULL, IDC_ARROW);	//设置一个箭头光标
	WindowsClass.hIcon = nullptr;						//应用程序放在磁盘上显示的图标
	WindowsClass.hIconSm = nullptr;						//应用程序显示在左上角的图标
	WindowsClass.hInstance = InParameters.HInstance;	//传递自己的窗口实例
	WindowsClass.lpszClassName = (LPCSTR)_T("LurenjiaEngine");		//窗口名称
	WindowsClass.lpszMenuName = nullptr;				//设置菜单的名称
	WindowsClass.style = CS_GLOBALCLASS;				//怎么绘制窗口，垂直或者水平
	WindowsClass.lpfnWndProc = EngineWindowProc;		//绑定消息处理函数

	//注册窗口
	ATOM RegisterAtom = RegisterClassEx(&WindowsClass);
	if (!RegisterAtom)
	{
		// 如果注册失败
		Engine_Log_Error("Register windows class failed.");
		MessageBox(NULL, (LPCSTR)_T("Register windows class fail,"), (LPCSTR)_T("Error"), MB_OK);
	}

	/*
	* {left, top, right, bottom}
	*/
	RECT Rect = { 0, 0, FEngineRenderConfig::GetRenderConfig()->ScreenWidth, FEngineRenderConfig::GetRenderConfig()->ScreenHeight };
	/*
	* 创建的视口
	* WS_OVERLAPPEDWINDOW 视口风格
	* 是否有菜单
	*/
	AdjustWindowRect(&Rect, WS_OVERLAPPEDWINDOW, NULL);

	int WindowWidth = Rect.right - Rect.left;
	int WindowHeight = Rect.bottom - Rect.top;

	MyWindowsHandle = CreateWindowEx(
		NULL,						//窗口额外风格
		"LurenjiaEngine",			//窗口名称
		"Lurenjia Engine",			//窗口名称(显示在窗口的标题栏上)
		WS_OVERLAPPEDWINDOW,		//窗口风格
		100, 100,					//窗口左上角起始位置
		WindowWidth, WindowHeight,	//窗口长宽
		NULL,						//副窗口句柄
		nullptr,					//窗口菜单句柄
		InParameters.HInstance,		//窗口实例
		NULL						//可以往窗口中传参数
		);
	if (!MyWindowsHandle)
	{
		Engine_Log_Error("CreateWindow failed.");
		MessageBox(0, (LPCSTR)_T("CreateWindow Failed."), 0, 0);
		return false;
	}
	
	//显示窗口
	ShowWindow(MyWindowsHandle, SW_SHOW);
	//窗口是脏的就刷一下
	UpdateWindow(MyWindowsHandle);
	Engine_Log("InitWindows complete.");
	return true;
}
#endif
