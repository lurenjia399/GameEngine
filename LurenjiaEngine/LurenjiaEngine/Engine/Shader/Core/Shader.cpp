#include "Shader.h"

void FShader::BuildShader(const wstring& InFileName, const string& InEntryFunName, const string& InShadersVersion)
{
    ComPtr<ID3DBlob> ErrorShaderMsg;
    D3DCompileFromFile(
        InFileName.c_str(),
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        InEntryFunName.c_str(),
        InShadersVersion.c_str(),
#if _DEBUG
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
#else
        0,
#endif  
        0,//如何编译我们的效果
        ShaderCode.GetAddressOf(),
        ErrorShaderMsg.GetAddressOf());
    if (ErrorShaderMsg)
    {
        Engine_Log_Error("FShader::BuildShader error[%s]", (char*)ErrorShaderMsg->GetBufferPointer());
    }
}
