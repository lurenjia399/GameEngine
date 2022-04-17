#pragma once
#include "../../LurenjiaEngine.h"

class FShader
{
public:
	/// <summary>
	/// ������ɫ��
	/// </summary>
	/// <param name="InFileName">��ɫ���ļ�λ��</param>
	/// <param name="InEntryFunName">��ɫ���ļ�����ں���</param>
	/// <param name="InShadersVersion">��ɫ���汾</param>
	void BuildShader(const wstring& InFileName, const string& InEntryFunName, const string& InShadersVersion);
	LPVOID GetBufferPointer(void);
	SIZE_T GetBufferSize(void);
private:
	ComPtr<ID3DBlob> ShaderCode;
};