#pragma once

#ifndef OUTLINEREDITOR_H
#define OUTLINEREDITOR_H

#include "../../Engine/LurenjiaEngine.h"
#include "../Core/EditorBase.h"
#include "../Engine/Interface/DirectXDeviceInterface.h"

class FOutlinerEditor : public FEditorBase, public IDirectXDeviceInterface
{
public:

	virtual void BuildEditor()  override;				// 重写FEditorBase的
	virtual void DrawEditor(float DeltaTime)  override;	// 重写FEditorBase的
	virtual void ExitEditor()  override;				// 重写FEditorBase的


public:
};



#endif