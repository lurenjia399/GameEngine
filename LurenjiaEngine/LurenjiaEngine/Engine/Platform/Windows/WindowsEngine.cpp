#include "WindowsEngine.h"
#if defined(_WIN32)
#include "WindowsMessageProcessing.h"
int FWindowsEngine::PreInit(FWinMainCommandParameters InParameters)
{
	//��������

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
	// ע�ᴰ��
	WNDCLASSEX WindowsClass;
	WindowsClass.cbSize = sizeof(WNDCLASSEX);			//�ö���ʵ��ռ�ö���ڴ�
	WindowsClass.cbClsExtra = 0;						//�Ƿ���Ҫ����ռ�
	WindowsClass.cbWndExtra = 0;						//�Ƿ���Ҫ�����ڴ�
	WindowsClass.hbrBackground = nullptr;				//��������þ���GDI����
	WindowsClass.hCursor = LoadCursor(NULL, IDC_ARROW);	//����һ����ͷ���
	WindowsClass.hIcon = nullptr;						//Ӧ�ó�����ڴ�������ʾ��ͼ��
	WindowsClass.hIconSm = nullptr;						//Ӧ�ó�����ʾ�����Ͻǵ�ͼ��
	WindowsClass.hInstance = InParameters.HInstance;	//�����Լ��Ĵ���ʵ��
	WindowsClass.lpszClassName = L"LurenjiaEngine";		//��������
	WindowsClass.lpszMenuName = nullptr;				//���ò˵�������
	WindowsClass.style = CS_VREDRAW | CS_HREDRAW;		//��ô���ƴ��ڣ���ֱ����ˮƽ
	WindowsClass.lpfnWndProc = EngineWindowProc;		//����Ϣ������

	//ע�ᴰ��
	ATOM RegisterAtom = RegisterClassEx(&WindowsClass);
	if (!RegisterAtom)
	{
		// ���ע��ʧ��
		MessageBox(NULL, L"Register fail,", L"Error", MB_OK);
	}

	/*
	* {left, top, right, bottom}
	*/
	RECT Rect = { 0, 0, 1280, 720 };
	/*
	* �������ӿ�
	* WS_OVERLAPPEDWINDOW �ӿڷ��
	* �Ƿ��в˵�
	*/
	AdjustWindowRect(&Rect, WS_OVERLAPPEDWINDOW, NULL);

	int WindowWidth = Rect.right - Rect.left;
	int WindowHeight = Rect.bottom - Rect.top;

	HWND Hwnd = CreateWindowEx(
		NULL,						//���ڶ�����
		L"LurenjiaEngine",			//��������
		L"Lurenjia Engine",			//��������(��ʾ�ڴ��ڵı�������)
		WS_OVERLAPPEDWINDOW,		//���ڷ��
		100, 100,					//�������Ͻ���ʼλ��
		WindowWidth, WindowHeight,	//���ڳ���
		NULL,						//�����ھ��
		nullptr,					//���ڲ˵����
		InParameters.HInstance,		//����ʵ��
		NULL						//�����������д�����
		);
	if (!Hwnd)
	{
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}
	
	//��ʾ����
	ShowWindow(Hwnd, SW_SHOW);
	//��������ľ�ˢһ��
	UpdateWindow(Hwnd);
	//return false;
}
#endif
