#pragma once
#include <iostream>
using namespace fbxsdk;

namespace FbxImport
{
	struct FbxRenderData
	{

	};
	
	void LoadMeshData(const std::string& InPath, FbxRenderData& OutRenderData);
}