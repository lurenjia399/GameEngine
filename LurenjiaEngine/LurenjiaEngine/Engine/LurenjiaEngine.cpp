#include "LurenjiaEngine.h"
#include "EngineFactory.h"

static int Init(CEngine* InEngine, HINSTANCE InhInstance, HINSTANCE InprevInstance, PSTR IncmdLine, int InshowCmd)
{
#if defined(_WIN32)
	FWinMainCommandParameters WinMainParameters(InhInstance, InprevInstance, IncmdLine, InshowCmd);
#endif
//	int ReturnValue = InEngine->PreInit(
//#if defined(_WIN32)
//		WinMainParameters
//#endif
//	);
//	if (ReturnValue != 0)
//	{
//		Engine_Log_Error("[%i]Engine Pre initialization error, please check and initialization problem.", ReturnValue);
//		return ReturnValue;
//	}
	int ReturnValue = InEngine->Init(
#if defined(_WIN32)
		WinMainParameters
#endif
	);
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine initialization error, please check and initialization problem.", ReturnValue);
		return ReturnValue;
	}
	//ReturnValue = InEngine->PostInit();
	//if (ReturnValue != 0)
	//{
	//	Engine_Log_Error("[%i]Engine post initialization error, please check and initialization problem.", ReturnValue);
	//	return ReturnValue;
	//}
	return ReturnValue;
}

static void Tick(CEngine* InEngine)
{
	float DeltaTime = 0.03f;
	InEngine->Tick(DeltaTime);
	Sleep(30);
}

static int Exit(CEngine* InEngine)
{
	//int ReturnValue = InEngine->PreExit();
	//if (ReturnValue != 0)
	//{
	//	Engine_Log_Error("[%i]Engine pre exit failed", ReturnValue);
	//	return ReturnValue;
	//}
	int ReturnValue = InEngine->Exit();
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine exit failed", ReturnValue);
		return ReturnValue;
	}
	//ReturnValue = InEngine->PostExit();
	//if (ReturnValue != 0)
	//{
	//	Engine_Log_Error("[%i]Engine post exit failed", ReturnValue);
	//	return ReturnValue;
	//}
	return ReturnValue;
}

CEngine* Engine = nullptr;
/*
* hInstance �Լ���ʵ��
* prevInstance �ϴε�ʵ��
* cmdLine ���ݵ�����
* showCmd ��ʲô��ʽ��ʾ
*	//showCmd				������
	//SW_HIDE				���ش��ڲ���������һ������
	//SW_MINIMIZE			��С��ָ�����ڣ����Ҽ�����ϵͳ���еĶ��㴰��
	//SW_RESTORE			�����ʾ���ڡ���������Ѿ���С������󻯣�ϵͳ���ָ���ԭ���ĳߴ��λ����ʾ���ڣ���SW_SHOWNORMAL��ͬ��
	//SW_SHOW				����һ�����ڲ���ԭ���ĳߴ��λ����ʾ����
	//SW_SHOWMAXIMIZED		����ڲ��������
	//SW_SHOWMINIMIZED		����ڲ�������С��
	//SW_SHOWMINNOACTIVE	��һ��������Ϊͼ�ꡣ�����ά�ֻ״̬��
	//SW_SHOWNA				�Դ��ڵĵ�ǰ״̬��ʾ���ڡ�����ڱ��ֻ״̬
	//SW_SHOWNOACTIVATE		�Դ��ڵ����һ�εĳߴ��λ����ʾ���ڡ��������
	//SW_SHOWNORMAL			�����ʾ���ڣ���������Ѿ���С������󻯣�ϵͳ���ָ���ԭ���ĳߴ��λ����ʾ����
*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	int ReturnValue = 0;
	Engine = FEngineFactory::CreateEngine();
	if (Engine)
	{
		ReturnValue = Init(Engine, hInstance, prevInstance, cmdLine, showCmd);
		
		MSG EngineMsg = { 0 };

		while (EngineMsg.message != WM_QUIT)
		{
			//PM_NOREMOVE		��Ϣ���Ƴ���Ϣ����
			//PM_REMOVE			��Ϣ�Ƴ���Ϣ����
			//PM_NOYIELD		�˱�־ʹϵͳ���ͷŵȴ����ó�����е��߳�
			//PM_QS_INPUT		������������Ϣ
			//PM_QS_PAINT		����ͼ��Ϣ
			//PM_QS_POSTMESSAGE	�������б����͵���Ϣ��������ʱ�����ȼ�
			//PM_QS_SENDMESSAGE	�������Ƿ��͵���Ϣ
			if (PeekMessage(&EngineMsg, 0, 0, 0, PM_REMOVE))
			{
				//����Ϣ������ַ���
				TranslateMessage(&EngineMsg);
				DispatchMessage(&EngineMsg);
			}
			else {
				Tick(Engine);
			}
		}
		ReturnValue = Exit(Engine);
	}
	else {
		ReturnValue = 1;
	}
	Engine_Log("[&i]The engine has exited.", ReturnValue);
	return ReturnValue;
}