#pragma once

#ifndef TOOLBAREDITOR_H
#define TOOLBAREDITOR_H

#include "../../Engine/LurenjiaEngine.h"
#include "../Core/EditorBase.h"

class FToolbarEditor : public FEditorBase
{
public:

	virtual void BuildEditor()  override;				// 重写FEditorBase的
	virtual void DrawEditor(float DeltaTime)  override;	// 重写FEditorBase的
	virtual void ExitEditor()  override;				// 重写FEditorBase的


public:
};



#endif