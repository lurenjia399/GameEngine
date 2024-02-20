#pragma once

#ifndef LOGEDITOR_H
#define TOOLBAREDITOR_H

#include "../../Engine/Core/Engine.h"
#include "../Core/EditorBase.h"

class FLogEditor : public FEditorBase
{
public:

	virtual void BuildEditor()  override;				// 重写FEditorBase的
	virtual void DrawEditor(float DeltaTime)  override;	// 重写FEditorBase的
	virtual void ExitEditor()  override;				// 重写FEditorBase的
};


#endif