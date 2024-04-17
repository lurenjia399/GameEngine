#include "CustomMeshComponent.h"
#include "FBX/FBXMain.h"

CCustomMeshComponent::CCustomMeshComponent()
{
}

void CCustomMeshComponent::CreateMeshRenderData(FVertexRenderingData& InRenderingData, const string& InPath)
{
	//局部代码块
	{
		unsigned int FileSize = get_file_size_by_filename(InPath.c_str());

		char* Buffer = new char[FileSize + 1]();//永远记住字符数组的长度需要加1，，，'\0'需要占位
		get_file_buf(InPath.c_str(), Buffer);

		bool bLoadSuccess = false;
		if (InPath.find(".fbx") != -1)
		{
			bLoadSuccess = LoadFBXFileBuffer(InPath, InRenderingData);
		}
		else
		{
			bLoadSuccess = LoadObjFileBuffer(Buffer, FileSize, InRenderingData);
		}
		if (!bLoadSuccess)
		{
			Engine_Log_Error("load obj mesh failed !!!");
		}
		else {
			cout << endl;
		}

		delete[] Buffer;
		_ASSERTE(_CrtCheckMemory());//检测对堆的数据操作是否溢出
	}
}

void CCustomMeshComponent::BuildKey(size_t& OutHashKey, const string& InPath)
{
	std::hash<string> stringHash;

	OutHashKey = stringHash(InPath);
}

bool CCustomMeshComponent::LoadObjFileBuffer(char* InBuffer, uint32_t InBufferSize, FVertexRenderingData& OutRenderingData)
{
	if (InBufferSize > 0)
	{
		stringstream BufferStream(InBuffer);
		char TempLine[256 + 1] = { '\0' };
		string garbageTag = "";
		for (; !BufferStream.eof(); )
		{
			memset(TempLine, 0, 256 + 1);
			BufferStream.getline(TempLine, 256 + 1);
			if (strlen(TempLine) > 0)
			{
				if (TempLine[0] == 'v')
				{
					stringstream LineStream(TempLine);
					LineStream >> garbageTag;

					if (TempLine[1] == 'n')		//法线
					{

					}
					else if (TempLine[1] == 't')//贴图
					{

					}
					else {						//顶点
						OutRenderingData.VertexData.emplace_back(FVertex(XMFLOAT3(), XMFLOAT4(Colors::DeepSkyBlue)));
						int topIndex = static_cast<int>(OutRenderingData.VertexData.size()) - 1;
						XMFLOAT3& currVertexPos = OutRenderingData.VertexData[topIndex].Pos;

						//解析顶点位置
						LineStream >> currVertexPos.x;
						LineStream >> currVertexPos.y;
						LineStream >> currVertexPos.z;
					}
				}
				else if (TempLine[0] == 'f')
				{
					stringstream LineStream(TempLine);
					LineStream >> garbageTag;

					char SaveLineString[257] = { '\0' };
					char TempBuffer[257] = { '\0' };
					for (size_t i = 0; i < 3; i++)
					{
						memset(SaveLineString, 0, 257);
						LineStream >> SaveLineString;
						//顶点索引放到数据中
						memset(TempBuffer, 0, 257);
						int pivotIndex = 0;
						int PosIndex = find_string(SaveLineString, "/", pivotIndex);
						if (PosIndex < 0)
						{
							OutRenderingData.IndexData.emplace_back(atoi(SaveLineString) - 1);
						}
						else {
							char* vPosIndex = string_mid(SaveLineString, TempBuffer, pivotIndex, PosIndex - pivotIndex);
							OutRenderingData.IndexData.emplace_back(atoi(vPosIndex) - 1);
						}

						//纹理索引
						memset(TempBuffer, 0, 257);
						pivotIndex = PosIndex + 1;
						int TextureIndex = find_string(SaveLineString, "/", pivotIndex);
						if (TextureIndex < 0)
						{

						}
						else {
							char* vTextureIndex = string_mid(SaveLineString, TempBuffer, pivotIndex, TextureIndex - pivotIndex);
						}

						//法线索引
						memset(TempBuffer, 0, 257);
						pivotIndex = TextureIndex + 1;
						char* vNormalIndex = string_mid(SaveLineString, TempBuffer, pivotIndex, static_cast<int>(strlen(SaveLineString)) - pivotIndex);
					}
				}
			}
		}
		return true;
	}
	return false;
}

bool CCustomMeshComponent::LoadFBXFileBuffer(const std::string& InPath, FVertexRenderingData& OutRenderingData)
{
	FbxImport::FbxRenderData FbxSceneRenderData = {};
	FbxImport::LoadMeshData(InPath, FbxSceneRenderData);

	for (const FbxImport::FFbxModel& ModelData : *FbxSceneRenderData.ModelData)
	{
		for (const FbxImport::FFbxPolygon& PolygonData : *ModelData.PolygonData)
		{
			for (const FbxImport::FFbxTriangle& TriangleData : *PolygonData.VertexData)
			{
				for (const FbxImport::FFbxVertex& VertexData : TriangleData.Vertexs)
				{
					XMFLOAT3 Pos = XMFLOAT3(VertexData.Position.x, VertexData.Position.y, VertexData.Position.z);
					XMFLOAT4 Color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
					XMFLOAT3 Normal = XMFLOAT3(VertexData.Normal.x, VertexData.Normal.y, VertexData.Normal.z);
					XMFLOAT3 Tangent = XMFLOAT3(VertexData.Tangent.x, VertexData.Tangent.y, VertexData.Tangent.z);;
					XMFLOAT2 TexCoord = XMFLOAT2(VertexData.UV.x, VertexData.UV.y);
					OutRenderingData.VertexData.emplace_back(FVertex(Pos, Color, Normal, Tangent, TexCoord));
				}
			}
		}
	}
	return false;
}