#pragma once

#ifndef SELECTEDITOR_H
#define SELECTEDITOR_H

#include "../../Engine/LurenjiaEngine.h"
#include "../Core/EditorBase.h"

class FSelectEditor : public FEditorBase
{
public:

	virtual void BuildEditor()  override;				// 重写FEditorBase的
	virtual void DrawEditor(float DeltaTime)  override;	// 重写FEditorBase的
	virtual void ExitEditor()  override;				// 重写FEditorBase的


public:
};



#endif