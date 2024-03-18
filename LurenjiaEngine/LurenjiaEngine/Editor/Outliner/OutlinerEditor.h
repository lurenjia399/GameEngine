#pragma once

#ifndef OUTLINEREDITOR_H
#define OUTLINEREDITOR_H

#include "../../Engine/LurenjiaEngine.h"
#include "../Core/EditorBase.h"

class FOutlinerEditor : public FEditorBase
{
public:

	virtual void BuildEditor()  override;				// ��дFEditorBase��
	virtual void DrawEditor(float DeltaTime)  override;	// ��дFEditorBase��
	virtual void ExitEditor()  override;				// ��дFEditorBase��


public:
};



#endif