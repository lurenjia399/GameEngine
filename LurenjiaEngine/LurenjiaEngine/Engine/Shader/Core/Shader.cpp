#include "Shader.h"

void FShader::BuildShader(const wstring& InFileName, const string& InEntryFunName, const string& InShadersVersion)
{
    ComPtr<ID3DBlob> ErrorShaderMsg;
    HRESULT R = D3DCompileFromFile(
        InFileName.c_str(),
        NULL,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        InEntryFunName.c_str(),
        InShadersVersion.c_str(),
#if defined(_DEBUG)
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
#else
        0,
#endif  
        0,//如何编译我们的效果
        &ShaderCode,
        &ErrorShaderMsg);


    if (ErrorShaderMsg != nullptr)
    {
        Engine_Log_Error("FShader::BuildShader error[%s]", (char*)ErrorShaderMsg->GetBufferPointer());
    }
    ANALYSIS_HRESULT(R);
}

LPVOID FShader::GetBufferPointer(void) const
{
    assert(ShaderCode.Get() != nullptr);
    return ShaderCode->GetBufferPointer();
}

SIZE_T FShader::GetBufferSize(void) const
{
    assert(ShaderCode.Get() != nullptr);
    return ShaderCode->GetBufferSize();
}
