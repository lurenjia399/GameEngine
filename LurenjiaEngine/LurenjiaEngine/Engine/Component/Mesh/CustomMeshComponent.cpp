#include "CustomMeshComponent.h"

CCustomMeshComponent::CCustomMeshComponent()
{
}

void CCustomMeshComponent::CreateMeshRenderData(FMeshRenderingData& InRenderingData, const string& InPath)
{
	//局部代码块
	{
		unsigned int FileSize = get_file_size_by_filename(InPath.c_str());

		char* Buffer = new char[FileSize + 1]();//永远记住字符数组的长度需要加1，，，'\0'需要占位
		get_file_buf(InPath.c_str(), Buffer);

		if (!LoadObjFileBuffer(Buffer, FileSize, InRenderingData))
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

bool CCustomMeshComponent::LoadObjFileBuffer(char* InBuffer, uint32_t InBufferSize, FMeshRenderingData& InRenderingData)
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
						InRenderingData.VertexData.emplace_back(FVertex(XMFLOAT3(), XMFLOAT4(Colors::DeepSkyBlue)));
						int topIndex = InRenderingData.VertexData.size() - 1;
						XMFLOAT3& currVertexPos = InRenderingData.VertexData[topIndex].Pos;

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
							InRenderingData.IndexData.emplace_back(atoi(SaveLineString) - 1);
						}
						else {
							char* vPosIndex = string_mid(SaveLineString, TempBuffer, pivotIndex, PosIndex - pivotIndex);
							InRenderingData.IndexData.emplace_back(atoi(vPosIndex) - 1);
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
						char* vNormalIndex = string_mid(SaveLineString, TempBuffer, pivotIndex, strlen(SaveLineString) - pivotIndex);
					}
				}
			}
		}
		return true;
	}
	return false;
}
