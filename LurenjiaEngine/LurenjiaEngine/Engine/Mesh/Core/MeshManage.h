#pragma once
#include "../../Core/CoreObject/CoreMinimalObject.h"
//#include "../../Rendering/Core/Rendering.h"
#include "../../Rendering/Core/RenderingResourcesUpdate.h"
#include "MeshType.h"
#include "../../Shader/Core/Shader.h"
#include "Mesh.h"
#include "../../Interface/DirectXDeviceInterface.h"
#include "../../Rendering/Core/Buffer/ConstructBuffer.h"


class CMeshManage
	: public CCoreMinimalObject
	, public IRenderingInterface
	, public ConstructBuffer::FConstructBuffer
{
public:
	CMeshManage();
	virtual void Init() override;

	virtual void BuildMesh(const FMeshRenderingData* InRenderingData);

	virtual void PreDraw(float DeltaTime) override;
	virtual void Draw(float DeltaTime) override;
	virtual void PostDraw(float DeltaTime) override;

	void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);
public:
	/// <summary>
	/// 创建BoxMesh
	/// </summary>
	/// <param name="InName">名称</param>
	/// <param name="Inheight">高度</param>
	/// <param name="Inwidth">宽度</param>
	/// <param name="Indepth">深度</param>
	/// <returns>Mesh基类</returns>
	CMesh* CreateBoxMesh(string InName, const float& Inheight, const float& Inwidth, const float& Indepth);
	/// <summary>
	/// 创建平面Mesh
	/// </summary>
	/// <param name="InName">名称</param>
	/// <param name="Inheight">高度</param>
	/// <param name="Inwidth">宽度</param>
	/// <param name="InHeightSubdivide">高度细分</param>
	/// <param name="InwidthSubdivide">宽度细分</param>
	/// <returns>Mesh基类</returns>
	CMesh* CreatePlaneMesh(string InName, const float& Inheight, const float& Inwidth, const uint32_t& InHeightSubdivide, const uint32_t& InwidthSubdivide);
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
	CMesh* CreateCylinderMesh(string InName, const float& InTopRadius, const float& InBottomRadius, const uint32_t& InAxialSubdivision, const float& InHeight, const uint32_t& InHeightSubdivision);
	/// <summary>
	/// 创建OBJMesh
	/// </summary>
	/// <param name="InName">名称</param>
	/// <param name="InPath">模型路径</param>
	/// <returns>Mesh基类</returns>
	CMesh* CreateCustomMesh(string InName, const string& InPath);
	/// <summary>
	/// 创建球
	/// </summary>
	/// <param name="InName">名称</param>
	/// <param name="InRadius">半径</param>
	/// <param name="InAxialSubdivision">轴向细分</param>
	/// <param name="InHeightSubdivision">高度细分</param>
	/// <returns>Mesh基类</returns>
	CMesh* CreateSphereMesh(string InName, const float& InRadius, const uint32_t& InAxialSubdivision, const uint32_t& InHeightSubdivision);
private:
	template<typename T, typename S, typename ...ParamTypes>
	T* CreateMesh(const S& name, ParamTypes&&... Params);
private:
	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView();
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView();
private:
	ComPtr<ID3DBlob> CPUVertexBufferPtr;					//CPU存放mesh的顶点缓冲区
	ComPtr<ID3DBlob> CPUIndexBufferPtr;						//CPU存放mesh的索引缓冲区

	ComPtr<ID3D12Resource> GPUVertexBufferPtr;				//GPUmesh的顶点缓冲区
	ComPtr<ID3D12Resource> GPUIndexBufferPtr;				//GPUmesh的索引缓冲区

	ComPtr<ID3D12Resource> VertexBufferTempPtr;				//临时mesh的顶点缓冲区
	ComPtr<ID3D12Resource> IndexBufferTempPtr;				//临时mesh的索引缓冲区


	ComPtr<ID3D12DescriptorHeap> CBVHeap;					//常量描述堆
	shared_ptr<FRenderingResourcesUpdate> objectConstants;	//对象的常量缓冲区
	shared_ptr<FRenderingResourcesUpdate> viewportConstants;	//viewport的常量缓冲区

	ComPtr<ID3D12RootSignature> RootSignature;				//根签名

	FShader VertexShader;									//顶点着色器
	FShader PixelShader;									//片元着色器
	vector<D3D12_INPUT_ELEMENT_DESC> InputElementDesc;		//着色器输入布局

	ComPtr<ID3D12PipelineState> PSO;						//管线状态

private:
	UINT VertexSizeInBytes;									//顶点资源所占内存大小
	UINT VertexStrideInBytes;								//顶点步径大小（也就是每个顶点数据大小）
	UINT IndexSizeInBytes;									//索引资源所占内存大小
private:
	DXGI_FORMAT IndexFormat;								//索引资源格式
	UINT IndexSize;											//顶点的数量

	XMFLOAT4X4 WorldMatrix;									//mvp变换矩阵
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

	BuildMesh(&MeshRenderingData);

	mesh->Init();
	return mesh;
}