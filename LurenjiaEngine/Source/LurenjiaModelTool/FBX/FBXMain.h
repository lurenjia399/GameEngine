#pragma once
#include <iostream>
//using namespace fbxsdk;

namespace FbxImport
{
	struct FbxRenderData
	{

	};

	// 2.3.4
	struct FbxVersion
	{
		int Major;
		int Minor;
		int Revision;
	};
	
	void LoadMeshData(const std::string& InPath, FbxRenderData& OutRenderData);
}