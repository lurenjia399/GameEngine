#pragma once 
#include "../../../Core/Engine.h"

class CRenderingEngine : public CEngine
{
public:
	void SetMainWindowsHandle(HWND InNewMianWindowsHandle);
	HWND GetMainWindowsHandle() const {return MainWindowsHandle;}
protected:
	HWND MainWindowsHandle;
};