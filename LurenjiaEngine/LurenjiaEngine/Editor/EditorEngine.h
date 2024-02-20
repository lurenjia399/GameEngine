#pragma once

#ifndef EDITORENGINE_H
#define EDITORENGINE_H

#include "../Engine/Core/Engine.h"
#include "Core/EditorBase.h"

class FToolbarEditor;
class FLogEditor;

class CEditorEngine : public CEngine, public FEditorBase
{
public:
	CEditorEngine();


	virtual int PreInit(
#if defined(_WIN32)
		FWinMainCommandParameters& InParameters
#endif
	) override;
	virtual int Init(
#if defined(_WIN32)
		FWinMainCommandParameters InParameters
#endif
	) override;


	virtual int PostInit() override;

	virtual void Tick(float DeltaTime) override;

	virtual int PreExit() override;
	virtual int Exit() override;
	virtual int PostExit() override;

	virtual void BuildEditor()  override;				// ��дFEditorBase��
	virtual void DrawEditor(float DeltaTime)  override;	// ��дFEditorBase��
	virtual void ExitEditor()  override;				// ��дFEditorBase��

private:
	FToolbarEditor* ToolbarEditor;
	FLogEditor* LogEditor;

};




#endif