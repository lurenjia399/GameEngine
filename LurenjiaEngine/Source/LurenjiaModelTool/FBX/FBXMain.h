#pragma once
#include <iostream>
#include <vector>
#include <map>
#include "FBXCoreMacro.h"

namespace FbxImport
{
	// 二维向量
	struct FFbxVector2
	{
		FFbxVector2():x(0.f),y(0.f){}
		float x;
		float y;
	};
	// 三维向量
	struct FFbxVector3 : public FFbxVector2
	{
		FFbxVector3():z(0.f) {}
		float z;
	};
	// 顶点
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
	// 三角形
	struct FFbxTriangle
	{
		FFbxTriangle(): Vertexs(), MaterialID(0) {}
		FFbxVertex Vertexs[3];
		int MaterialID;
	};
	// 多边形
	struct FFbxPolygon
	{
		FFbxPolygon():VertexData({}), IndexData({}), MaterialID(0) {}
		std::vector<FFbxTriangle> VertexData;
		std::vector<uint16_t> IndexData;
		int MaterialID;
	};
	// 材质
	struct FFbxMaterial
	{
		FFbxMaterial():DiffuseMapFileName("") {}
		std::string DiffuseMapFileName;
	};
	// 模型
	struct FFbxModel
	{
		FFbxModel() :PolygonData({}), MaterialMap({}) {}
		std::vector<FFbxPolygon> PolygonData;
		std::map<int, FFbxMaterial> MaterialMap;
	};

	// 这个场景的渲染数据。场景 -> 多少个模型 -> 每个模型上多少个多边形(三角形) -> 每个三角形是三个顶点
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