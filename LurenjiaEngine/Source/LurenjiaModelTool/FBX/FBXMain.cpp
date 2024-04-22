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

namespace FbxImport
{
	
	
	
}


void DestoryFbxObjects(FbxManager* InManager)
{
	if (InManager)
	{
		InManager->Destroy();
	}
}

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
void GetPolygons(fbxsdk::FbxMesh* Mesh, FbxImport::FFbxPolygon& OutPolygonRenderData)
{
	int PolygonCount = Mesh->GetPolygonCount();//获取图元的数量,三角形的数量吧
	FbxVector4* ControlPoints = Mesh->GetControlPoints();//获取控制点的数量

	int VertexID = 0;
	for (int i = 0; i < PolygonCount; i++) // 有多少个图元
	{
		OutPolygonRenderData.push_vertex(FbxImport::FFbxTriangle());
		FbxImport::FFbxTriangle& OutTriangle = OutPolygonRenderData.pop_vertex_ref();

		int PolygonSize = Mesh->GetPolygonSize(i);//每个图元的尺寸，三角形就是3，四边形就是4
		assert(PolygonSize == 3);
		for (int j = 0; j < PolygonSize; j++)
		{
			FbxImport::FFbxVector3& OutPosition = OutTriangle.Vertexs[j].Position;
			FbxImport::FFbxVector2& OutUV = OutTriangle.Vertexs[j].UV;
			FbxImport::FFbxVector3& OutNormal = OutTriangle.Vertexs[j].Normal;
			FbxImport::FFbxVector3& OutTangent = OutTriangle.Vertexs[j].Tangent;
			FbxImport::FFbxVector3& OutBinormal = OutTriangle.Vertexs[j].Binormal;


			
			int ControlPointIndex = Mesh->GetPolygonVertex(i, j);//获取点的索引


			// position
			{
				// scale
				FbxDouble3 Scaling = Mesh->GetNode()->LclScaling;

				FbxVector4 Coordinates = ControlPoints[ControlPointIndex];
				OutPosition.x = static_cast<float>(Coordinates.mData[0]) * static_cast<float>(Scaling[0]);
				OutPosition.y = static_cast<float>(Coordinates.mData[1]) * static_cast<float>(Scaling[1]);
				OutPosition.z = static_cast<float>(Coordinates.mData[2]) * static_cast<float>(Scaling[2]);
			}

			// color
			for (int k = 0; k < Mesh->GetElementVertexColorCount(); k++)
			{

			}

			// uv
			for (int k = 0; k < Mesh->GetElementUVCount(); k++)
			{
				fbxsdk::FbxGeometryElementUV* TextureUV = Mesh->GetElementUV(k);
				if (TextureUV->GetMappingMode() == FbxGeometryElement::eByControlPoint)
				{
					if (TextureUV->GetReferenceMode() == FbxGeometryElement::eDirect)
					{
						// 获取顶点UV值
						FbxVector2 UV =  TextureUV->GetDirectArray().GetAt(ControlPointIndex);
						OutUV.x = static_cast<float>(UV.mData[0]);
						OutUV.y = 1.0f - static_cast<float>(UV.mData[1]);// dx中v是反的，opengl正好相反

					}else if(TextureUV->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
					{
						int ID = TextureUV->GetIndexArray().GetAt(ControlPointIndex);
						FbxVector2 UV = TextureUV->GetDirectArray().GetAt(ID);
						OutUV.x = static_cast<float>(UV.mData[0]);
						OutUV.y = 1.0f - static_cast<float>(UV.mData[1]);// dx中v是反的，opengl正好相反
					}
				}
				else if (TextureUV->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					int TextureUVIndex = Mesh->GetTextureUVIndex(i, j);
					FbxVector2 UV = TextureUV->GetDirectArray().GetAt(TextureUVIndex);
					if (TextureUV->GetReferenceMode() == FbxGeometryElement::eDirect 
						|| TextureUV->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
					{
						FbxVector2 UV = TextureUV->GetDirectArray().GetAt(ControlPointIndex);
						OutUV.x = static_cast<float>(UV.mData[0]);
						OutUV.y = 1.0f - static_cast<float>(UV.mData[1]);// dx中v是反的，opengl正好相反

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
						OutNormal.x = static_cast<float>(Normal.mData[0]);
						OutNormal.y = static_cast<float>(Normal.mData[1]);
						OutNormal.z = static_cast<float>(Normal.mData[2]);
					}
					else if (ElementNormal->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
					{
						int ID = ElementNormal->GetIndexArray().GetAt(VertexID);
						FbxVector4 Normal = ElementNormal->GetDirectArray().GetAt(ID);
						OutNormal.x = static_cast<float>(Normal.mData[0]);
						OutNormal.y = static_cast<float>(Normal.mData[1]);
						OutNormal.z = static_cast<float>(Normal.mData[2]);
					}
				}
				else if (ElementNormal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
				{
					if (ElementNormal->GetReferenceMode() == FbxGeometryElement::eDirect)
					{
						// 获取顶点法线
						FbxVector4 Normal = ElementNormal->GetDirectArray().GetAt(ControlPointIndex);
						OutNormal.x = static_cast<float>(Normal.mData[0]);
						OutNormal.y = static_cast<float>(Normal.mData[1]);
						OutNormal.z = static_cast<float>(Normal.mData[2]);
					}
					else if (ElementNormal->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
					{
						int ID = ElementNormal->GetIndexArray().GetAt(ControlPointIndex);
						FbxVector4 Normal = ElementNormal->GetDirectArray().GetAt(ID);
						OutNormal.x = static_cast<float>(Normal.mData[0]);
						OutNormal.y = static_cast<float>(Normal.mData[1]);
						OutNormal.z = static_cast<float>(Normal.mData[2]);
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
						FbxVector4 Tangent = ElementTangent->GetDirectArray().GetAt(VertexID);
						OutTangent.x = static_cast<float>(Tangent.mData[0]);
						OutTangent.y = static_cast<float>(Tangent.mData[1]);
						OutTangent.z = static_cast<float>(Tangent.mData[2]);
					}
					else if (ElementTangent->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
					{
						int TangentIndex = ElementTangent->GetIndexArray().GetAt(VertexID);
						FbxVector4 Tangent = ElementTangent->GetDirectArray().GetAt(TangentIndex);
						OutTangent.x = static_cast<float>(Tangent.mData[0]);
						OutTangent.y = static_cast<float>(Tangent.mData[1]);
						OutTangent.z = static_cast<float>(Tangent.mData[2]);
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
						FbxVector4 Binormal = ElementBinormal->GetDirectArray().GetAt(VertexID);
						OutBinormal.x = static_cast<float>(Binormal.mData[0]);
						OutBinormal.y = static_cast<float>(Binormal.mData[1]);
						OutBinormal.z = static_cast<float>(Binormal.mData[2]);
					}
					else if (ElementBinormal->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
					{
						int BinormalIndex = ElementBinormal->GetIndexArray().GetAt(VertexID);
						FbxVector4 Binormal = ElementBinormal->GetDirectArray().GetAt(BinormalIndex);
						OutBinormal.x = static_cast<float>(Binormal.mData[0]);
						OutBinormal.y = static_cast<float>(Binormal.mData[1]);
						OutBinormal.z = static_cast<float>(Binormal.mData[2]);
					}
				}
			}

			VertexID++;
		}
	}
}

void GetIndex(fbxsdk::FbxMesh* Mesh, FbxImport::FFbxPolygon& OutPolygonRenderData)
{
	uint16_t IndexDataSize = static_cast<uint16_t>(OutPolygonRenderData.imp->VertexData.size()) * 3;
	OutPolygonRenderData.imp->IndexData.resize(IndexDataSize);
	for (uint16_t i = 0; i < IndexDataSize; ++i)
	{
		OutPolygonRenderData.imp->IndexData[i] = i;
	}
}

void GetMesh(FbxNode* Node, FbxImport::FFbxModel& OutModelRenderData)
{
	fbxsdk::FbxMesh* NodeMesh = (fbxsdk::FbxMesh*)Node->GetNodeAttribute();

	OutModelRenderData.push_polygon(FbxImport::FFbxPolygon());
	FbxImport::FFbxPolygon& OutPolygon = OutModelRenderData.pop_polygon_ref();
	GetPolygons(NodeMesh, OutPolygon);
	GetIndex(NodeMesh, OutPolygon);
}

void RecursiveLoadMesh(FbxNode* Node, FbxImport::FFbxRenderData& OutRenderData)
{
	if (Node->GetNodeAttribute() != NULL)
	{
		FbxNodeAttribute::EType AttributeType = Node->GetNodeAttribute()->GetAttributeType();
		if (AttributeType == FbxNodeAttribute::EType::eMesh)
		{
			//FbxImport::FFbxModel model = ;
			OutRenderData.push_model(FbxImport::FFbxModel());
			FbxImport::FFbxModel& OutModel = OutRenderData.pop_model_ref();
			GetMesh(Node, OutModel);
		}
	}
}

void FbxImport::LoadMeshData(const std::string& InPath, FbxImport::FFbxRenderData& OutRenderData)
{
	FbxManager* Manager = nullptr;
	FbxScene* Scene = nullptr;

	InitializeObjects(Manager, Scene);

	FbxString FBXPath(InPath.c_str());
	bool Result = LoadScene(Manager, Scene, FBXPath.Buffer());//通过Fbxanager把fbx模型加载到FbxScene里面了

	// 将fbx场景转成dx的场景
	//FbxAxisSystem::DirectX.ConvertScene(Scene);//这边一堆的编译错，等下看

	if (FbxNode* Node = Scene->GetRootNode())
	{
		for (int i = 0; i < Node->GetChildCount(); i++)
		{
			RecursiveLoadMesh(Node->GetChild(i), OutRenderData);
		}
	}

	DestoryFbxObjects(Manager);
}
