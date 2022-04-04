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
* hInstance 自己的实列
* prevInstance 上次的实例
* cmdLine 传递的命令
* showCmd 已什么方式显示
*	//showCmd				参数：
	//SW_HIDE				隐藏窗口并激活另外一个窗口
	//SW_MINIMIZE			最小化指定窗口，并且激活在系统表中的顶层窗口
	//SW_RESTORE			激活并显示窗口。如果窗口已经最小化或最大化，系统将恢复到原来的尺寸和位置显示窗口（与SW_SHOWNORMAL相同）
	//SW_SHOW				激活一个窗口并以原来的尺寸和位置显示窗口
	//SW_SHOWMAXIMIZED		激活窗口并将其最大化
	//SW_SHOWMINIMIZED		激活窗口并将其最小化
	//SW_SHOWMINNOACTIVE	将一个窗口设为图标。激活窗口维持活动状态。
	//SW_SHOWNA				以窗口的当前状态显示窗口。激活窗口保持活动状态
	//SW_SHOWNOACTIVATE		以窗口的最近一次的尺寸和位置显示窗口。不激活窗口
	//SW_SHOWNORMAL			激活并显示窗口，如果窗口已经最小化或最大化，系统将恢复到原来的尺寸和位置显示窗口
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