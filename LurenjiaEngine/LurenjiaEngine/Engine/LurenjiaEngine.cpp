#include "LurenjiaEngine.h"
#include "EngineFactory.h"
#include "Debug/Log/SimpleLog.h"

int Init(FEngine* InEngine, HINSTANCE InhInstance, HINSTANCE InprevInstance, PSTR IncmdLine, int InshowCmd)
{
	int ReturnValue = InEngine->PreInit(
#if defined(_WIN32)
		FWinMainCommandParameters(InhInstance, InprevInstance, IncmdLine, InshowCmd)
#endif
	);
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine Pre initialization error, please check and initialization problem.", ReturnValue);
		return ReturnValue;
	}
	ReturnValue = InEngine->Init();
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine initialization error, please check and initialization problem.", ReturnValue);
		return ReturnValue;
	}
	ReturnValue = InEngine->PostInit();
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine post initialization error, please check and initialization problem.", ReturnValue);
		return ReturnValue;
	}
	return ReturnValue;
}

void Tick(FEngine* InEngine)
{
	InEngine->Tick();
}

int Exit(FEngine* InEngine)
{
	int ReturnValue = InEngine->PreExit();
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine pre exit failed", ReturnValue);
		return ReturnValue;
	}
	ReturnValue = InEngine->Exit();
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine exit failed", ReturnValue);
		return ReturnValue;
	}
	ReturnValue = InEngine->PostExit();
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine post exit failed", ReturnValue);
		return ReturnValue;
	}
	return ReturnValue;
}

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
	if (FEngine* Engine = FEngineFactory::CreateEngine())
	{
		
		ReturnValue = Init(Engine, hInstance, prevInstance, cmdLine, showCmd);
		
		while (true)
		{
			Tick(Engine);
		}

		ReturnValue = Exit(Engine);
	}
	else {
		ReturnValue = 1;
	}
	Engine_Log("[&i]The engine has exited.", ReturnValue);
	return ReturnValue;
}