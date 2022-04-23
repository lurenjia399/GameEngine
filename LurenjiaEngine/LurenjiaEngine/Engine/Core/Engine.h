#pragma once
#include "CoreObject/CoreMinimalObject.h"

#if defined(_WIN32) // �����windowsƽ̨�£�������ִ���������Ĵ���
#include "WinMainCommandParameters.h"


#endif // defined(_WIN32)

struct FViewportInfo;

class CEngine : public CCoreMinimalObject
{
public:
	CEngine();
	virtual int PreInit(
#if defined(_WIN32)
		FWinMainCommandParameters& InParameters
#endif
	) = 0;
	virtual int Init(
#if defined(_WIN32)
		FWinMainCommandParameters InParameters
#endif
	) = 0;
	virtual int PostInit() = 0;

	virtual void Tick(float DeltaTime) = 0;//�˴��麯����ѻ����tick�ڸ�ס���Ӷ��޷����õ������tick

	virtual int PreExit() = 0;
	virtual int Exit() = 0;
	virtual int PostExit() = 0;
	virtual void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo) {}
};