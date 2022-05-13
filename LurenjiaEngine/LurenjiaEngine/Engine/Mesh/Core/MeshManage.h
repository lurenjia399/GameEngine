#pragma once
#include "../../Core/CoreObject/CoreMinimalObject.h"
//#include "../../Rendering/Core/Rendering.h"
#include "../../Rendering/Core/RenderingResourcesUpdate.h"
#include "MeshType.h"
#include "../../Shader/Core/Shader.h"
#include "Mesh.h"
#include "../../Interface/DirectXDeviceInterface.h"
#include "../../Rendering/Core/Buffer/ConstructBuffer.h"
#include "../../Rendering/Core/DirectX/RenderingPipeline/RenderingPipeline.h"


class CMeshManage
	: public CCoreMinimalObject
	, public IRenderingInterface
	, public ConstructBuffer::FConstructBuffer
{
public:
	CMeshManage();
	virtual void Init() override;

	virtual void BuildPipeline();

	virtual void PreDraw(float DeltaTime) override;
	virtual void Draw(float DeltaTime) override;
	virtual void PostDraw(float DeltaTime) override;

	void UpdateConstantView(float DeltaTime, const FViewportInfo& ViewportInfo);
public:
	/// <summary>
	/// 创建BoxMesh
	/// </summary>
	/// <param name="InName">名称</param>
	/// <param name="Inheight">高度</param>
	/// <param name="Inwidth">宽度</param>
	/// <param name="Indepth">深度</param>
	/// <returns>Mesh基类</returns>
	AMesh* CreateBoxMesh(string InName, const float& Inheight, const float& Inwidth, const float& Indepth);
	/// <summary>
	/// 创建平面Mesh
	/// </summary>
	/// <param name="InName">名称</param>
	/// <param name="Inheight">高度</param>
	/// <param name="Inwidth">宽度</param>
	/// <param name="InHeightSubdivide">高度细分</param>
	/// <param name="InwidthSubdivide">宽度细分</param>
	/// <returns>Mesh基类</returns>
	AMesh* CreatePlaneMesh(string InName, const float& Inheight, const float& Inwidth, const uint32_t& InHeightSubdivide, const uint32_t& InwidthSubdivide);
	/// <summary>
	/// 创建柱形Mesh
	/// </summary>
	/// <param name="InName">名称</param>
	/// <param name="InTopRadius">顶半径</param>
	/// <param name="InBottomRadius">底半径</param>
	/// <param name="InAxialSubdivision">轴向细分</param>
	/// <param name="InHeight">高度</param>
	/// <param name="InHeightSubdivision">高度细分</param>
	/// <returns>Mesh基类</returns>
	AMesh* CreateCylinderMesh(string InName, const float& InTopRadius, const float& InBottomRadius, const uint32_t& InAxialSubdivision, const float& InHeight, const uint32_t& InHeightSubdivision);
	/// <summary>
	/// 创建OBJMesh
	/// </summary>
	/// <param name="InName">名称</param>
	/// <param name="InPath">模型路径</param>
	/// <returns>Mesh基类</returns>
	AMesh* CreateCustomMesh(string InName, const string& InPath);
	/// <summary>
	/// 创建球
	/// </summary>
	/// <param name="InName">名称</param>
	/// <param name="InRadius">半径</param>
	/// <param name="InAxialSubdivision">轴向细分</param>
	/// <param name="InHeightSubdivision">高度细分</param>
	/// <returns>Mesh基类</returns>
	AMesh* CreateSphereMesh(string InName, const float& InRadius, const uint32_t& InAxialSubdivision, const uint32_t& InHeightSubdivision);
private:
	template<typename T, typename S, typename ...ParamTypes>
	T* CreateMesh(const S& name, ParamTypes&&... Params);
private:
	FRenderingPipeline RenderingPipeline;					//渲染管线对象




};
//----------模板实现-----
template<typename T>
string getName(T name)
{
	return name;
}
template<typename T, typename S, typename ...ParamTypes>
T* CMeshManage::CreateMesh(const S& name, ParamTypes&&... Params)
{
	T* mesh = new T();
	mesh->ResetGuid(name);

	FMeshRenderingData MeshRenderingData;
	mesh->CreateMeshRenderData(MeshRenderingData, std::forward<ParamTypes>(Params)...);
	mesh->BeginInit();

	RenderingPipeline.BuildMesh(mesh, MeshRenderingData);

	mesh->Init();
	return mesh;
}