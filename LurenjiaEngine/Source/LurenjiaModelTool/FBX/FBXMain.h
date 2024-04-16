#pragma once
#include <iostream>
#include <vector>
#include <map>
#include "FBXCoreMacro.h"

namespace FbxImport
{
	// ��ά����
	struct FFbxVector2
	{
		FFbxVector2():x(0.f),y(0.f){}
		float x;
		float y;
	};
	// ��ά����
	struct FFbxVector3 : public FFbxVector2
	{
		FFbxVector3():z(0.f) {}
		float z;
	};
	// ����
	struct FFbxVertex
	{
		FFbxVertex(): Position({}), Color({}), Normal({}), Tangent({}), Binormal({}), UV({}) {}
		FFbxVector3 Position;
		FFbxVector3 Color;
		FFbxVector3 Normal;
		FFbxVector3 Tangent;
		FFbxVector3 Binormal;
		FFbxVector2 UV;

	};
	// ������
	struct FFbxTriangle
	{
		FFbxTriangle(): Vertexs(), MaterialID(0) {}
		FFbxVertex Vertexs[3];
		int MaterialID;
	};
	// �����
	struct FFbxPolygon
	{
		FFbxPolygon():VertexData({}), IndexData({}), MaterialID(0) {}
		std::vector<FFbxTriangle> VertexData;
		std::vector<uint16_t> IndexData;
		int MaterialID;
	};
	// ����
	struct FFbxMaterial
	{
		FFbxMaterial():DiffuseMapFileName("") {}
		std::string DiffuseMapFileName;
	};
	// ģ��
	struct FFbxModel
	{
		FFbxModel() :PolygonData({}), MaterialMap({}) {}
		std::vector<FFbxPolygon> PolygonData;
		std::map<int, FFbxMaterial> MaterialMap;
	};

	// �����������Ⱦ���ݡ����� -> ���ٸ�ģ�� -> ÿ��ģ���϶��ٸ������(������) -> ÿ������������������
	struct LURENJIAENMODELTOOL_API FbxRenderData
	{
		FbxRenderData():ModelData(nullptr){};
		~FbxRenderData()
		{
			if (ModelData != nullptr)
			{
				delete ModelData;
			}
			
		}
		std::vector<FFbxModel>* ModelData;
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