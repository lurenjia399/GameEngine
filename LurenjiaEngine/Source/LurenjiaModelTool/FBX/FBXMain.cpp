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
		OutPolygonRenderData.VertexData.push_back(FbxImport::FFbxTriangle());
		FbxImport::FFbxTriangle& OutTriangle = OutPolygonRenderData.VertexData.back();

		int PolygonSize = Mesh->GetPolygonSize(i);//ÿ��ͼԪ�ĳߴ磬�����ξ���3���ı��ξ���4
		assert(PolygonSize == 3);
		for (int j = 0; j < PolygonSize; j++)
		{
			FbxImport::FFbxVector3& OutPosition = OutTriangle.Vertexs[j].Position;
			FbxImport::FFbxVector2& OutUV = OutTriangle.Vertexs[j].UV;

			
			int ControlPointIndex = Mesh->GetPolygonVertex(i, j);//��ȡ�������

			// position
			{
				FbxVector4 Coordinates = ControlPoints[ControlPointIndex];
				OutPosition.x = static_cast<float>(Coordinates.mData[0]);
				OutPosition.y = static_cast<float>(Coordinates.mData[1]);
				OutPosition.z = static_cast<float>(Coordinates.mData[2]);
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
						int TangentIndex = ElementTangent->GetIndexArray().GetAt(VertexID);
						// ��ȡ��������
						FbxVector4 Tangent =  ElementTangent->GetDirectArray().GetAt(TangentIndex);
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
						int BinormalIndex = ElementBinormal->GetIndexArray().GetAt(VertexID);
						// ��ȡ���㸱����
						//FbxVector4 Binormal = ElementBinormal->GetDirectArray().GetAt(BinormalIndex);
					}
				}
			}

			VertexID++;
		}
	}
}

void GetMesh(FbxNode* Node, FbxImport::FFbxModel& OutModelRenderData)
{
	fbxsdk::FbxMesh* NodeMesh = (fbxsdk::FbxMesh*)Node->GetNodeAttribute();

	OutModelRenderData.PolygonData.push_back(FbxImport::FFbxPolygon());
	FbxImport::FFbxPolygon& OutPolygon = OutModelRenderData.PolygonData.back();
	GetPolygons(NodeMesh, OutPolygon);
}

void RecursiveLoadMesh(FbxNode* Node, FbxImport::FbxRenderData& OutRenderData)
{
	if (Node->GetNodeAttribute() != NULL)
	{
		FbxNodeAttribute::EType AttributeType = Node->GetNodeAttribute()->GetAttributeType();
		if (AttributeType == FbxNodeAttribute::EType::eMesh)
		{
			(*OutRenderData.ModelData).push_back(FbxImport::FFbxModel());
			FbxImport::FFbxModel& OutModel = (*OutRenderData.ModelData).back();
			GetMesh(Node, OutModel);
		}
	}
}

void FbxImport::LoadMeshData(const std::string& InPath, FbxImport::FbxRenderData& OutRenderData)
{
	FbxManager* Manager = nullptr;
	FbxScene* Scene = nullptr;

	InitializeObjects(Manager, Scene);

	FbxString FBXPath(InPath.c_str());
	bool Result = LoadScene(Manager, Scene, FBXPath.Buffer());//ͨ��Fbxanager��fbxģ�ͼ��ص�FbxScene������

	if (FbxNode* Node = Scene->GetRootNode())
	{
		for (int i = 0; i < Node->GetChildCount(); i++)
		{
			RecursiveLoadMesh(Node->GetChild(i), OutRenderData);
		}
	}

	DestoryFbxObjects(Manager);
}
