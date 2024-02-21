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
	void Clear(); // ���log
	void AddLog(const char* Fmt, ...); //���log
	void AddLogSuccess(const char* Fmt, ...); //���log
	void AddLogError(const char* Fmt, ...); //���log
	void AddLogWarning(const char* Fmt, ...); //���log

protected:
	void Draw(float DeltaTime); // ����
protected:
	ImGuiTextBuffer TextBuffer;	// һ�������ڴ棬ͨ��\n���ָ��ַ���
	ImGuiTextFilter TextFilter;	// log��������Ǻ����
	ImVector<int> LineOffset;	// ����\n��λ�ã����Ե�����
	bool bAutoScroll;			// �Ƿ��Զ�����log
private:
	FEditorLogSystem();

	static FEditorLogSystem* EditorLogSystem;
};

#endif