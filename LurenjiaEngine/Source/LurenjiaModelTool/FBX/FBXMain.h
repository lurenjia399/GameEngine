#pragma once
#include <iostream>
#include <vector>
#include <map>
#include "FBXCoreMacro.h"

namespace FbxImport
{
	// ��ά����
	struct LURENJIAENMODELTOOL_API FFbxVector2
	{
		FFbxVector2():x(0.f),y(0.f){}
		float x;
		float y;
	};
	// ��ά����
	struct LURENJIAENMODELTOOL_API FFbxVector3 : public FFbxVector2
	{
		FFbxVector3():z(0.f) {}
		float z;
	};
	// ����
	struct LURENJIAENMODELTOOL_API FFbxVertex
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
	struct LURENJIAENMODELTOOL_API FFbxTriangle
	{
		FFbxTriangle(): Vertexs(), MaterialID(0) {}
		FFbxVertex Vertexs[3];
		int MaterialID;
	};
	// �����
	struct LURENJIAENMODELTOOL_API FFbxPolygon
	{
		FFbxPolygon():VertexData(new std::vector<FFbxTriangle>{}), IndexData(new std::vector<uint16_t>{}), MaterialID(0) {}
		~FFbxPolygon()
		{
			if (VertexData != nullptr)
			{
				delete VertexData;
			}
			if (IndexData != nullptr)
			{
				delete IndexData;
			}

		}
		std::vector<FFbxTriangle>* VertexData;
		std::vector<uint16_t>* IndexData;
		int MaterialID;
	};
	// ����
	struct LURENJIAENMODELTOOL_API FFbxMaterial
	{
		FFbxMaterial() :DiffuseMapFileName("") {}
		const char* DiffuseMapFileName;
	};
	// ģ��
	struct LURENJIAENMODELTOOL_API FFbxModel
	{
		FFbxModel() :PolygonData(new std::vector<FFbxPolygon>{}), MaterialMap(new std::map<int, FFbxMaterial>{}) {}
		~FFbxModel()
		{
			if (PolygonData != nullptr)
			{
				delete PolygonData;
			}
			if (MaterialMap != nullptr)
			{
				delete MaterialMap;
			}

		}
		std::vector<FFbxPolygon>* PolygonData;
		std::map<int, FFbxMaterial>* MaterialMap;
	};

	// �����������Ⱦ���ݡ����� -> ���ٸ�ģ�� -> ÿ��ģ���϶��ٸ������(������) -> ÿ������������������
	struct LURENJIAENMODELTOOL_API FbxRenderData
	{
		FbxRenderData():ModelData(new std::vector<FFbxModel>{}){};
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