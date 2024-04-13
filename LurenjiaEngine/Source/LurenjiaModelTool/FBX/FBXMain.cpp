#include "FBXMain.h"

#include "include/fbxsdk.h"
#pragma comment(lib, "libfbxsdk.lib")
#pragma comment(lib, "libfbxsdk-md.lib")
#pragma comment(lib, "libfbxsdk-mt.lib")

#pragma comment(lib, "LurenjiaEngineCore.lib")
#include "Path/EnginePath.h"

using namespace fbxsdk;
//int main()
//{
//	//FbxManager* InManager = FbxManager::Create();
//	FbxImport::FbxRenderData RenderData{};
//	string ContentPath = FEnginePathHelper::GetEngineContentFBXPath();
//	std::string path = ContentPath + "/Heart.fbx";
//	FbxImport::LoadMeshData(path, RenderData);
//	return 0;
//}

void InitializeObjects(FbxManager*& OutManager, FbxScene*& OutScene)
{
	OutManager = FbxManager::Create(); // 创建FbxManager

	FbxIOSettings* IO = FbxIOSettings::Create(OutManager, IOSROOT);
	OutManager->SetIOSettings(IO);

	FbxString FBXPath = FbxGetApplicationDirectory();
	OutManager->LoadPluginsDirectory(FBXPath);

	OutScene = FbxScene::Create(OutManager, "FbxScene");// 创建场景
}

bool LoadScene(FbxManager* FbxManager, FbxScene* FbxScene, const char* FilePath)
{
	FbxImport::FbxVersion Version = {};
	FbxManager::GetFileFormatVersion(Version.Major, Version.Minor, Version.Revision);// 创建FBX版本

	fbxsdk::FbxImporter* FBXImporterPtr = fbxsdk::FbxImporter::Create(FbxManager, "FBXImporter");
	bool result = FBXImporterPtr->Initialize(FilePath, -1, FbxManager->GetIOSettings());

	FbxImport::FbxVersion FileVersion = {};
	FBXImporterPtr->GetFileVersion(FileVersion.Major, FileVersion.Minor, FileVersion.Revision);

	if (!result)
	{
		return false;
	}

	if (FBXImporterPtr->IsFBX())
	{

	}
	result = FBXImporterPtr->Import(FbxScene);
	if (result && FBXImporterPtr->GetStatus().GetCode() == FbxStatus::ePasswordError)
	{
		// 密码错误
	}
	FBXImporterPtr->Destroy();

	return result;
}

// 获取多边形
void GetPolygons(fbxsdk::FbxMesh* Mesh, FbxImport::FbxRenderData& OutRenderDat)
{
	int PolygonCount = Mesh->GetPolygonCount();//获取图元的数量,三角形的数量吧
	FbxVector4* ControlPoints = Mesh->GetControlPoints();//获取控制点的数量

	int VertexID = 0;
	for (int i = 0; i < PolygonCount; i++)
	{
		int PolygonSize = Mesh->GetPolygonSize(i);//图元的size，三角形就是3，四边形就是4
		for (int j = 0; j < PolygonSize; j++)
		{
			int ControlPointIndex = Mesh->GetPolygonVertex(i, j);//获取点的索引
			// 获取顶点的坐标
			FbxVector4 Coordinates = ControlPoints[ControlPointIndex];

			// uv
			for (int k = 0; k < Mesh->GetElementUVCount(); k++)
			{
				fbxsdk::FbxGeometryElementUV* TextureUV = Mesh->GetElementUV(k);
				if (TextureUV->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					int TextureUVIndex = Mesh->GetTextureUVIndex(i, j);
					if (TextureUV->GetReferenceMode() == FbxGeometryElement::eIndex)
					{
						// 获取顶点UV值
						FbxVector2 UV =  TextureUV->GetDirectArray().GetAt(TextureUVIndex);
					}
				}
			}
			// 法线
			for (int k = 0; k < Mesh->GetElementNormalCount(); k++)
			{
				fbxsdk::FbxGeometryElementNormal* ElementNormal = Mesh->GetElementNormal(k);
				if (ElementNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					if (ElementNormal->GetReferenceMode() == FbxGeometryElement::eDirect)
					{
						// 获取顶点法线
						FbxVector4 Normal = ElementNormal->GetDirectArray().GetAt(VertexID);
					}
				}
			}
			// 切线T
			for (int k = 0; k < Mesh->GetElementTangentCount(); k++)
			{
				fbxsdk::FbxGeometryElementTangent* ElementTangent = Mesh->GetElementTangent(k);
				if (ElementTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					if (ElementTangent->GetReferenceMode() == FbxGeometryElement::eDirect)
					{
						int TangentIndex = ElementTangent->GetIndexArray().GetAt(VertexID);
						// 获取顶点切线
						FbxVector4 Tangent =  ElementTangent->GetDirectArray().GetAt(TangentIndex);
					}
				}
			}
			// 副切线B
			for (int k = 0; k < Mesh->GetElementBinormalCount(); k++)
			{
				fbxsdk::FbxGeometryElementBinormal* ElementBinormal = Mesh->GetElementBinormal(k);
				if (ElementBinormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					if (ElementBinormal->GetReferenceMode() == FbxGeometryElement::eDirect)
					{
						int BinormalIndex = ElementBinormal->GetIndexArray().GetAt(VertexID);
						// 获取顶点副切线
						//FbxVector4 Binormal = ElementBinormal->GetDirectArray().GetAt(BinormalIndex);
					}
				}
			}

			VertexID++;
		}
	}
}

void GetMesh(FbxNode* Node, FbxImport::FbxRenderData& OutRenderData)
{
	fbxsdk::FbxMesh* NodeMesh = (fbxsdk::FbxMesh*)Node->GetNodeAttribute();
	GetPolygons(NodeMesh, OutRenderData);
}

void RecursiveLoadMesh(FbxNode* Node, FbxImport::FbxRenderData& OutRenderData)
{
	if (Node->GetNodeAttribute() != NULL)
	{
		FbxNodeAttribute::EType AttributeType = Node->GetNodeAttribute()->GetAttributeType();
		if (AttributeType == FbxNodeAttribute::EType::eMesh)
		{
			GetMesh(Node, OutRenderData);
		}
	}
}

void FbxImport::LoadMeshData(const std::string& InPath, FbxImport::FbxRenderData& OutRenderData)
{
	FbxManager* Manager = nullptr;
	FbxScene* Scene = nullptr;

	InitializeObjects(Manager, Scene);

	FbxString FBXPath(InPath.c_str());
	bool Result = LoadScene(Manager, Scene, FBXPath.Buffer());//通过Fbxanager把fbx模型加载到FbxScene里面了

	if (FbxNode* Node = Scene->GetRootNode())
	{
		for (int i = 0; i < Node->GetChildCount(); i++)
		{
			RecursiveLoadMesh(Node->GetChild(i), OutRenderData);
		}
	}
}
