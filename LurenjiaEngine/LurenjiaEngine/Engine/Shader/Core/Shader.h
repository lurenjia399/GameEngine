#pragma once
#include "../../LurenjiaEngine.h"

class FShader
{
public:
	/// <summary>
	/// 构建着色器
	/// </summary>
	/// <param name="InFileName">着色器文件位置</param>
	/// <param name="InEntryFunName">着色器文件的入口函数</param>
	/// <param name="InShadersVersion">着色器版本</param>
	void BuildShader(const wstring& InFileName, const string& InEntryFunName, const string& InShadersVersion);
	LPVOID GetBufferPointer(void);
	SIZE_T GetBufferSize(void);
private:
	ComPtr<ID3DBlob> ShaderCode;
};