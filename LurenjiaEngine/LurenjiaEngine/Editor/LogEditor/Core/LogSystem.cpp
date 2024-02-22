#include "LogSystem.h"


FEditorLogSystem* FEditorLogSystem::EditorLogSystem = nullptr;

void FEditorLogSystem::Clear()
{
	TextBuffer.clear();
	LineOffset.clear();
	LineOffset.push_back(0);
}

void FEditorLogSystem::AddLog(const char* Fmt, ...)
{
	int curTextSize = TextBuffer.size();

	// va_list用法
	// https://blog.csdn.net/dengzhilong_cpp/article/details/54944676
	va_list Args = {};		// 定义一个va_list变量Args
	va_start(Args, Fmt);	// 执行Args = (va_list)&Fmt + _INTSIZEOF(Fmt)，Args指向参数Fmt之后的那个参数的地址，即 Args指向第一个可变参数在堆栈的地址。 
	TextBuffer.appendfv(Fmt, Args);
	va_end(Args);			// 清空va_list Args

	TextBuffer.append("\n");

	for (int newTextSize = TextBuffer.size(); curTextSize < newTextSize; ++curTextSize)
	{
		if (TextBuffer[curTextSize] == '\n')
		{
			LineOffset.push_back(curTextSize + 1);
		}
	}
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
	
	const char* TextBuffStart = TextBuffer.begin();
	const char* TextBuffEnd = TextBuffer.end();

	{
		// 按钮部分
		if (ImGui::Button("ClearAllLog"))
		{
			Clear();
		}
		ImGui::SameLine();
		if (ImGui::Button("CopyAllLog"))
		{
			ImGui::LogToClipboard();
		}
		ImGui::SameLine();
		// 绘制筛选器
		TextFilter.Draw("LogFilter", -100.f);
	}
	ImGui::Separator();

	ImGui::BeginChild("LogScrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
	{
		
		// 如果filter里面有东西
		if (TextFilter.IsActive())
		{
			for (int i = 0; i < LineOffset.Size; ++i)
			{
				const char* LineStart = TextBuffStart + LineOffset[i];
				const char* LineEnd = (i + 1 < LineOffset.Size ? (TextBuffStart + LineOffset[i + 1] - 1) : TextBuffEnd);
				if (TextFilter.PassFilter(LineStart, LineEnd))
				{
					ImGui::TextUnformatted(LineStart, LineEnd);
				}
			}
		}
		else
		{
			ImGuiListClipper Clipper = {};
			Clipper.Begin(LineOffset.size());
			while (Clipper.Step())
			{
				for (int i = Clipper.DisplayStart; i < Clipper.DisplayEnd; ++i)
				{
					const char* LineStart = TextBuffStart + LineOffset[i];
					const char* LineEnd = (i + 1 < LineOffset.Size ? (TextBuffStart + LineOffset[i + 1] - 1) : TextBuffEnd);
					ImGui::TextUnformatted(LineStart, LineEnd);
				}
			}
			Clipper.End();
		}

		/* {// 这样写也能实现过滤的效果，就是没用ImGuiListClipper
			for (int i = 0; i < LineOffset.Size; ++i)
			{
				const char* LineStart = TextBuffStart + LineOffset[i];
				const char* LineEnd = (i + 1 < LineOffset.Size ? (TextBuffStart + LineOffset[i + 1] - 1) : TextBuffEnd);
				if (TextFilter.IsActive())
				{
					if (TextFilter.PassFilter(LineStart, LineEnd))
					{
						ImGui::TextUnformatted(LineStart, LineEnd);
					}
				}
				else
				{
					ImGui::TextUnformatted(LineStart, LineEnd);
				}

			}
		}*/

		if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		{
			ImGui::SetScrollHereY(1.0f);
		}
		
	}
	


	ImGui::EndChild();

}

FEditorLogSystem::FEditorLogSystem()
	: TextBuffer({})
	, TextFilter(ImGuiTextFilter())
	, LineOffset({})
	, bAutoScroll(true)

{

}
