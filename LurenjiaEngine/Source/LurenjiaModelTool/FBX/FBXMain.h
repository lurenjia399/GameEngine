#pragma once
#include <iostream>
#include <vector>
#include <map>
#include "FBXCoreMacro.h"
#include <assert.h>


// https://www.cnblogs.com/YZFHKMS-X/p/11834989.html 导出dll，写的方式

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
	struct FFbxPolygon_imp
	{
		FFbxPolygon_imp() :VertexData({}), IndexData({}), MaterialID(0) {}
		void push_vertex(FFbxTriangle const& triangle)
		{
			VertexData.push_back(triangle);
		}
		FFbxTriangle& pop_vertex_ref()
		{
			assert(!VertexData.empty());
			return VertexData.back();
		}
		void push_index(uint16_t const& index)
		{
			IndexData.push_back(index);
		}
		uint16_t& pop_index_ref()
		{
			return IndexData.back();
		}
		std::vector<FFbxTriangle> VertexData;
		std::vector<uint16_t> IndexData;
		int MaterialID;
	};
	struct LURENJIAENMODELTOOL_API FFbxPolygon
	{
		FFbxPolygon()
		{
			imp = new FFbxPolygon_imp();
		}
		FFbxPolygon(const FFbxPolygon& polygon)
		{
			this->imp = new FFbxPolygon_imp();
			this->imp->VertexData = polygon.imp->VertexData;
			this->imp->IndexData = polygon.imp->IndexData;
			this->imp->MaterialID = polygon.imp->MaterialID;
		}
		~FFbxPolygon()
		{
			if (imp != nullptr)
			{
				delete imp;
			}
		}
		void push_vertex(FFbxTriangle const& triangle)
		{
			imp->push_vertex(triangle);
		}
		FFbxTriangle& pop_vertex_ref()
		{
			return imp->pop_vertex_ref();
		}
		void push_index(uint16_t const& index)
		{
			imp->push_index(index);
		}
		uint16_t& pop_index_ref()
		{
			return imp->pop_index_ref();
		}
		FFbxPolygon_imp* imp;
	};
	
	// 材质
	struct LURENJIAENMODELTOOL_API FFbxMaterial
	{
		FFbxMaterial() :DiffuseMapFileName("") {}
		const char* DiffuseMapFileName;
	};

	// 模型
	struct FFbxModel_imp
	{
		FFbxModel_imp() :PolygonData({}), MaterialMap({}) {}
		void push_polygon(FFbxPolygon const& polygon)
		{
			PolygonData.push_back(polygon);
		}
		FFbxPolygon& pop_polygon_ref()
		{
			assert(!PolygonData.empty());
			return PolygonData.back();
		}
		void push_material(int const& key, FFbxMaterial const& value)
		{
			MaterialMap.emplace(std::pair<int, FFbxMaterial>(key, value));
		}
		std::vector<FFbxPolygon> PolygonData;
		std::map<int, FFbxMaterial> MaterialMap;
	};
	struct LURENJIAENMODELTOOL_API FFbxModel
	{
		FFbxModel()
		{
			imp = new FFbxModel_imp();
		}
		FFbxModel(const FFbxModel& model)
		{
			this->imp = new FFbxModel_imp();
			this->imp->PolygonData = model.imp->PolygonData;
			this->imp->MaterialMap = model.imp->MaterialMap;
		}

		~FFbxModel()
		{
			if (imp != nullptr)
			{
				delete imp;
			}

		}
		void push_polygon(FFbxPolygon const& polygon)
		{
			imp->push_polygon(polygon);
		}
		FFbxPolygon& pop_polygon_ref()
		{
			return imp->pop_polygon_ref();
		}
		void push_material(int const& key, FFbxMaterial const& value)
		{
			imp->push_material(key, value);
		}
		FFbxModel_imp* imp;
	};
	
	// 这个场景的渲染数据。场景 -> 多少个模型 -> 每个模型上多少个多边形(三角形) -> 每个三角形是三个顶点
	struct FFbxRenderData_imp
	{
		FFbxRenderData_imp() :ModelData({}) {}
		void push_model(const FFbxModel& model)
		{
			ModelData.push_back(model);
		}
		FFbxModel& pop_model_ref()
		{
			assert(!ModelData.empty());
			return ModelData.back();
		}
		std::vector<FFbxModel> ModelData;
	};
	struct LURENJIAENMODELTOOL_API FFbxRenderData
	{
		FFbxRenderData()
		{
			imp = new FFbxRenderData_imp();
		};
		FFbxRenderData(FFbxRenderData const& renderData)
		{
			this->imp = new FFbxRenderData_imp();
			this->imp->ModelData = renderData.imp->ModelData;
		};
		~FFbxRenderData()
		{
			if (imp != nullptr)
			{
				delete imp;
			}
		}
		void push_model(const FFbxModel& model)
		{
			imp->push_model(model);
		}
		FFbxModel& pop_model_ref()
		{
			return imp->pop_model_ref();
		}

		FFbxRenderData_imp* imp;
	};
	

	// 2.3.4
	struct LURENJIAENMODELTOOL_API FbxVersion
	{
		int Major;
		int Minor;
		int Revision;
	};
	
	void LURENJIAENMODELTOOL_API LoadMeshData(const std::string& InPath, FFbxRenderData& OutRenderData);
}