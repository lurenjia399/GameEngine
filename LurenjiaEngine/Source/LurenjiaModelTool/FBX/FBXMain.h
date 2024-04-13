#pragma once
#include <iostream>
//using namespace fbxsdk;
#include "FBXCoreMacro.h"

namespace FbxImport
{
	struct LURENJIAENMODELTOOL_API FbxRenderData
	{

	};

	// 2.3.4
	struct LURENJIAENMODELTOOL_API FbxVersion
	{
		int Major;
		int Minor;
		int Revision;
	};
	
	void LURENJIAENMODELTOOL_API LoadMeshData(const std::string& InPath, FbxRenderData& OutRenderData);
}