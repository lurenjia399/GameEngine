#pragma once

#ifndef OUTLINEREDITOR_H
#define OUTLINEREDITOR_H

#include "../../Engine/LurenjiaEngine.h"
#include "../Core/EditorBase.h"
#include "../Engine/Interface/DirectXDeviceInterface.h"

class FOutlinerEditor : public FEditorBase, public IDirectXDeviceInterface
{
public:

	virtual void BuildEditor()  override;				// ��дFEditorBase��
	virtual void DrawEditor(float DeltaTime)  override;	// ��дFEditorBase��
	virtual void ExitEditor()  override;				// ��дFEditorBase��


public:
};



#endif