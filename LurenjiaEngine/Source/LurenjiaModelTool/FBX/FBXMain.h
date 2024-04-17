#pragma once
#include <iostream>
#include <vector>
#include <map>
#include "FBXCoreMacro.h"

namespace FbxImport
{
	// 二维向量
	struct LURENJIAENMODELTOOL_API FFbxVector2
	{
		FFbxVector2():x(0.f),y(0.f){}
		float x;
		float y;
	};
	// 三维向量
	struct LURENJIAENMODELTOOL_API FFbxVector3 : public FFbxVector2
	{
		FFbxVector3():z(0.f) {}
		float z;
	};
	// 顶点
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
	// 三角形
	struct LURENJIAENMODELTOOL_API FFbxTriangle
	{
		FFbxTriangle(): Vertexs(), MaterialID(0) {}
		FFbxVertex Vertexs[3];
		int MaterialID;
	};
	// 多边形
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
	// 材质
	struct LURENJIAENMODELTOOL_API FFbxMaterial
	{
		FFbxMaterial() :DiffuseMapFileName("") {}
		const char* DiffuseMapFileName;
	};
	// 模型
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

	// 这个场景的渲染数据。场景 -> 多少个模型 -> 每个模型上多少个多边形(三角形) -> 每个三角形是三个顶点
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