#include "ShaderType.h"

bool ShaderType::TransD3dShaderMacro(const std::vector<FShaderMacro>& inShaderMacro, std::vector<D3D_SHADER_MACRO>& OutShaderMacro)
{
	for (auto&& Temp : inShaderMacro)
	{
		D3D_SHADER_MACRO ShaderMacro =
		{
			Temp.Name.c_str(),
			Temp.Definition.c_str(),
		};
		OutShaderMacro.emplace_back(ShaderMacro);
	}
	if (!OutShaderMacro.empty())
	{
		D3D_SHADER_MACRO ShaderMacro = {};
		ShaderMacro.Name = NULL;
		ShaderMacro.Definition = NULL;

		OutShaderMacro.emplace_back(ShaderMacro);
	}
	return !OutShaderMacro.empty();
}
