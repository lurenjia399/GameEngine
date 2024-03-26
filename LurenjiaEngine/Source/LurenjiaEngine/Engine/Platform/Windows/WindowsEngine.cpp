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

	//��������
	Engine_Log("Engine pre initialization complete.");

	//��Ⱦ�����ʼ��
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
	// ע�ᴰ��
	WNDCLASSEX WindowsClass = {};
	WindowsClass.cbSize = sizeof(WNDCLASSEX);			//�ö���ʵ��ռ�ö���ڴ�
	WindowsClass.cbClsExtra = 0;						//�Ƿ���Ҫ����ռ�
	WindowsClass.cbWndExtra = 0;						//�Ƿ���Ҫ�����ڴ�
	WindowsClass.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);		//��ֹ���ĵı����ػ�
	WindowsClass.hCursor = LoadCursor(NULL, IDC_ARROW);	//����һ����ͷ���
	WindowsClass.hIcon = nullptr;						//Ӧ�ó�����ڴ�������ʾ��ͼ��
	WindowsClass.hIconSm = nullptr;						//Ӧ�ó�����ʾ�����Ͻǵ�ͼ��
	WindowsClass.hInstance = InParameters.HInstance;	//�����Լ��Ĵ���ʵ��
	WindowsClass.lpszClassName = (LPCSTR)_T("LurenjiaEngine");		//��������
	WindowsClass.lpszMenuName = nullptr;				//���ò˵�������
	WindowsClass.style = CS_GLOBALCLASS;				//��ô���ƴ��ڣ���ֱ����ˮƽ
	WindowsClass.lpfnWndProc = EngineWindowProc;		//����Ϣ������

	//ע�ᴰ��
	ATOM RegisterAtom = RegisterClassEx(&WindowsClass);
	if (!RegisterAtom)
	{
		// ���ע��ʧ��
		Engine_Log_Error("Register windows class failed.");
		MessageBox(NULL, (LPCSTR)_T("Register windows class fail,"), (LPCSTR)_T("Error"), MB_OK);
	}

	/*
	* {left, top, right, bottom}
	*/
	RECT Rect = { 0, 0, FEngineRenderConfig::GetRenderConfig()->ScreenWidth, FEngineRenderConfig::GetRenderConfig()->ScreenHeight };
	/*
	* �������ӿ�
	* WS_OVERLAPPEDWINDOW �ӿڷ��
	* �Ƿ��в˵�
	*/
	AdjustWindowRect(&Rect, WS_OVERLAPPEDWINDOW, NULL);

	int WindowWidth = Rect.right - Rect.left;
	int WindowHeight = Rect.bottom - Rect.top;

	MyWindowsHandle = CreateWindowEx(
		NULL,						//���ڶ�����
		"LurenjiaEngine",			//��������
		"Lurenjia Engine",			//��������(��ʾ�ڴ��ڵı�������)
		WS_OVERLAPPEDWINDOW,		//���ڷ��
		100, 100,					//�������Ͻ���ʼλ��
		WindowWidth, WindowHeight,	//���ڳ���
		NULL,						//�����ھ��
		nullptr,					//���ڲ˵����
		InParameters.HInstance,		//����ʵ��
		NULL						//�����������д�����
		);
	if (!MyWindowsHandle)
	{
		Engine_Log_Error("CreateWindow failed.");
		MessageBox(0, (LPCSTR)_T("CreateWindow Failed."), 0, 0);
		return false;
	}
	
	//��ʾ����
	ShowWindow(MyWindowsHandle, SW_SHOW);
	//��������ľ�ˢһ��
	UpdateWindow(MyWindowsHandle);
	Engine_Log("InitWindows complete.");
	return true;
}
#endif
