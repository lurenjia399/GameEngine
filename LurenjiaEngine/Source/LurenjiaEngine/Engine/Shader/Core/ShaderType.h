#pragma once

#ifndef SHADERTYPE_H
#define SHAERTYPE_H

#include "../../LurenjiaEngine.h"

namespace ShaderType
{
	struct FShaderMacro
	{
		std::string Name;
		std::string Definition;
	};

	bool TransD3dShaderMacro(const std::vector<FShaderMacro>& inShaderMacro, std::vector<D3D_SHADER_MACRO>& OutShaderMacro);
}


#endif
