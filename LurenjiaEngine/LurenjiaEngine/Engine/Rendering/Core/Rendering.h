#pragma once
#include "../../Core/Engine.h"
#include "../../Platform/Windows/WindowsEngine.h"

class IRenderingInterface //:virtual public IGuidInterface
{
public:
	IRenderingInterface();
	virtual ~IRenderingInterface() = 0;//��������������Ҫдʵ�֣����Ҵ����޷�ʵ����

	virtual void Init() = 0;

	virtual void PreDraw(float DeltaTime);
	virtual void Draw(float DeltaTime) = 0;
	virtual void PostDraw(float DeltaTime);
	
};
