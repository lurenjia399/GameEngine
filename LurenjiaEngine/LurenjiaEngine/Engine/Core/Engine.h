#pragma once
#include "CoreObject/CoreMinimalObject.h"

#if defined(_WIN32) // 如果是windows平台下，，，就执行括起来的代码
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

	virtual void Tick(float DeltaTime) = 0;//此纯虚函数会把基类的tick遮盖住，从而无法调用到基类的tick

	virtual int PreExit() = 0;
	virtual int Exit() = 0;
	virtual int PostExit() = 0;
	virtual void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo) {}
};