#pragma once

#ifndef EDITORBASE_H
#define EDITORBASE_H

#include "../../Engine/LurenjiaEngine.h"

class FEditorBase
{
public:
	virtual void BuildEditor() = 0;
	virtual void DrawEditor(float DeltaTime) = 0;
	virtual void ExitEditor() = 0;
};




#endif



