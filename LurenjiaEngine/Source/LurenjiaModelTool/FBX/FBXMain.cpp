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
	OutManager = FbxManager::Create(); // ����FbxManager

	FbxIOSettings* IO = FbxIOSettings::Create(OutManager, IOSROOT);
	OutManager->SetIOSettings(IO);

	FbxString FBXPath = FbxGetApplicationDirectory();
	OutManager->LoadPluginsDirectory(FBXPath);

	OutScene = FbxScene::Create(OutManager, "FbxScene");// ��������
}

bool LoadScene(FbxManager* FbxManager, FbxScene* FbxScene, const char* FilePath)
{
	FbxImport::FbxVersion Version = {};
	FbxManager::GetFileFormatVersion(Version.Major, Version.Minor, Version.Revision);// ����FBX�汾

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
		// �������
	}
	FBXImporterPtr->Destroy();

	return result;
}

// ��ȡ�����
void GetPolygons(fbxsdk::FbxMesh* Mesh, FbxImport::FFbxPolygon& OutPolygonRenderData)
{
	int PolygonCount = Mesh->GetPolygonCount();//��ȡͼԪ������,�����ε�������
	FbxVector4* ControlPoints = Mesh->GetControlPoints();//��ȡ���Ƶ������

	int VertexID = 0;
	for (int i = 0; i < PolygonCount; i++) // �ж��ٸ�ͼԪ
	{
		OutPolygonRenderData.push_vertex(FbxImport::FFbxTriangle());
		FbxImport::FFbxTriangle& OutTriangle = OutPolygonRenderData.pop_vertex_ref();

		int PolygonSize = Mesh->GetPolygonSize(i);//ÿ��ͼԪ�ĳߴ磬�����ξ���3���ı��ξ���4
		assert(PolygonSize == 3);
		for (int j = 0; j < PolygonSize; j++)
		{
			FbxImport::FFbxVector3& OutPosition = OutTriangle.Vertexs[j].Position;
			FbxImport::FFbxVector2& OutUV = OutTriangle.Vertexs[j].UV;
			FbxImport::FFbxVector3& OutNormal = OutTriangle.Vertexs[j].Normal;
			FbxImport::FFbxVector3& OutTangent = OutTriangle.Vertexs[j].Tangent;
			FbxImport::FFbxVector3& OutBinormal = OutTriangle.Vertexs[j].Binormal;


			
			int ControlPointIndex = Mesh->GetPolygonVertex(i, j);//��ȡ�������


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
						// ��ȡ����UVֵ
						FbxVector2 UV =  TextureUV->GetDirectArray().GetAt(ControlPointIndex);
						OutUV.x = static_cast<float>(UV.mData[0]);
						OutUV.y = 1.0f - static_cast<float>(UV.mData[1]);// dx��v�Ƿ��ģ�opengl�����෴

					}else if(TextureUV->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
					{
						int ID = TextureUV->GetIndexArray().GetAt(ControlPointIndex);
						FbxVector2 UV = TextureUV->GetDirectArray().GetAt(ID);
						OutUV.x = static_cast<float>(UV.mData[0]);
						OutUV.y = 1.0f - static_cast<float>(UV.mData[1]);// dx��v�Ƿ��ģ�opengl�����෴
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
						OutUV.y = 1.0f - static_cast<float>(UV.mData[1]);// dx��v�Ƿ��ģ�opengl�����෴

					}
				}
			}

			// ����
			for (int k = 0; k < Mesh->GetElementNormalCount(); k++)
			{
				fbxsdk::FbxGeometryElementNormal* ElementNormal = Mesh->GetElementNormal(k);
				if (ElementNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					if (ElementNormal->GetReferenceMode() == FbxGeometryElement::eDirect)
					{
						// ��ȡ���㷨��
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
						// ��ȡ���㷨��
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
			// ����T
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
			// ������B
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
	bool Result = LoadScene(Manager, Scene, FBXPath.Buffer());//ͨ��Fbxanager��fbxģ�ͼ��ص�FbxScene������

	// ��fbx����ת��dx�ĳ���
	//FbxAxisSystem::DirectX.ConvertScene(Scene);//���һ�ѵı�������¿�

	if (FbxNode* Node = Scene->GetRootNode())
	{
		for (int i = 0; i < Node->GetChildCount(); i++)
		{
			RecursiveLoadMesh(Node->GetChild(i), OutRenderData);
		}
	}

	DestoryFbxObjects(Manager);
}
