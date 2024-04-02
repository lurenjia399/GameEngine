#include "FBXMain.h"

#include "include/fbxsdk.h"
#pragma comment(lib, "libfbxsdk.lib")
#pragma comment(lib, "libfbxsdk-md.lib")
#pragma comment(lib, "libfbxsdk-mt.lib")

//int main()
//{
//	FbxManager* InManager = FbxManager::Create();
//	return 0;
//}

void InitializeObjects(FbxManager*& OutManager, FbxScene*& OutScene)
{
	OutManager = FbxManager::Create();

	FbxIOSettings* IO = FbxIOSettings::Create(OutManager, IOSROOT);
	OutManager->SetIOSettings(IO);
	//
}

void FbxImport::LoadMeshData(const std::string& InPath, FbxImport::FbxRenderData& OutRenderData)
{
	FbxManager* Manager = nullptr;
	FbxScene* Scene = nullptr;

	InitializeObjects(Manager, Scene);
}
