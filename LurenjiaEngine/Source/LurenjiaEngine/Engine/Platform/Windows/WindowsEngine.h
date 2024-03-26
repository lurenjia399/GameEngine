#pragma once

#if defined(_WIN32)
#include "../../Core/Engine.h"

class CWorld;
class CDirectXRenderingEngine;
class CEditorEngine;
class CWindowsEngine : public CEngine
{
public:
	CWindowsEngine();
	~CWindowsEngine();

	virtual int PreInit(FWinMainCommandParameters& InParameters) override;

	virtual int Init(FWinMainCommandParameters InParameters) override;
	virtual int PostInit() override;

	virtual void Tick(float DeltaTime) override;

	virtual int PreExit() override;
	virtual int Exit() override;
	virtual int PostExit() override;

	
	
//----------interface-----
	shared_ptr<CDirectXRenderingEngine> GetRenderingEngine() { return RenderingEngine; }
	shared_ptr<CEditorEngine> GetEditorEngine() { return EditorEngine; }
public:
	

protected:
	
//----------interface-----
private:
	bool InitWindows(FWinMainCommandParameters InParameters);
protected:
	HWND MyWindowsHandle;		//Ö÷´°¿Ú¾ä±ú
	shared_ptr<CDirectXRenderingEngine> RenderingEngine; // äÖÈ¾ÒýÇæ

#if (EDITOR_ENGINE == 1) 
	shared_ptr<CEditorEngine> EditorEngine; // ±à¼­Æ÷ÒýÇæ
#endif
};
#endif