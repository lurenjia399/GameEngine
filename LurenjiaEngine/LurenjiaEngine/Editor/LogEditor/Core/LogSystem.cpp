#include "LogSystem.h"


FEditorLogSystem* FEditorLogSystem::EditorLogSystem = nullptr;

void FEditorLogSystem::Clear()
{
}

void FEditorLogSystem::AddLog(const char* Fmt, ...)
{
}

void FEditorLogSystem::AddLogSuccess(const char* Fmt, ...)
{
}

void FEditorLogSystem::AddLogError(const char* Fmt, ...)
{
}

void FEditorLogSystem::AddLogWarning(const char* Fmt, ...)
{
}

void FEditorLogSystem::Draw(float DeltaTime)
{
}

FEditorLogSystem::FEditorLogSystem()
	: TextBuffer({})
	, TextFilter(ImGuiTextFilter())
	, LineOffset({})
	, bAutoScroll(true)

{

}
