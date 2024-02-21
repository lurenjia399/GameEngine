#pragma once

#ifndef LOGSYSTEM_H
#define LOGSYSTEM_H

#include "../../../Engine/LurenjiaEngine.h"
#include "../LogEditor.h"

class FLogEditor;

class FEditorLogSystem
{
	friend void FLogEditor::DrawEditor(float DeltaTime);
public:
	static FEditorLogSystem* GetEditorLogSystem()
	{
		if (EditorLogSystem == nullptr)
		{
			EditorLogSystem = new FEditorLogSystem();
		}

		return EditorLogSystem;
	}

	static void Destory()
	{
		if (EditorLogSystem != nullptr)
		{
			delete EditorLogSystem;
		}
		EditorLogSystem = nullptr;
	}


public:
	void Clear(); // 清除log
	void AddLog(const char* Fmt, ...); //添加log
	void AddLogSuccess(const char* Fmt, ...); //添加log
	void AddLogError(const char* Fmt, ...); //添加log
	void AddLogWarning(const char* Fmt, ...); //添加log

protected:
	void Draw(float DeltaTime); // 绘制
protected:
	ImGuiTextBuffer TextBuffer;	// 一个长的内存，通过\n来分割字符串
	ImGuiTextFilter TextFilter;	// log界面里的那红过滤
	ImVector<int> LineOffset;	// 决定\n的位置，可以调整下
	bool bAutoScroll;			// 是否自动滚动log
private:
	FEditorLogSystem();

	static FEditorLogSystem* EditorLogSystem;
};

#endif