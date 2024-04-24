#pragma once

#ifndef SELECTEDITOR_H
#define SELECTEDITOR_H

#include "../../Engine/LurenjiaEngine.h"
#include "../Core/EditorBase.h"

class FSelectEditor : public FEditorBase
{
public:

	virtual void BuildEditor()  override;				// ��дFEditorBase��
	virtual void DrawEditor(float DeltaTime)  override;	// ��дFEditorBase��
	virtual void ExitEditor()  override;				// ��дFEditorBase��


public:
};



#endif