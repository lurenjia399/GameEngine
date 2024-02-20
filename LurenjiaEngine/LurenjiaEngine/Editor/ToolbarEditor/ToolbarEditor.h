#pragma once

#ifndef TOOLBAREDITOR_H
#define TOOLBAREDITOR_H

#include "../../Engine/Core/Engine.h"
#include "../Core/EditorBase.h"

class FToolbarEditor : public FEditorBase
{
public:

	virtual void BuildEditor()  override;				// ��дFEditorBase��
	virtual void DrawEditor(float DeltaTime)  override;	// ��дFEditorBase��
	virtual void ExitEditor()  override;				// ��дFEditorBase��
};



#endif