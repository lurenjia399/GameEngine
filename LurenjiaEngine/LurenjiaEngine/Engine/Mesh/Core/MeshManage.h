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
	/// ����BoxMesh
	/// </summary>
	/// <param name="InName">����</param>
	/// <param name="Inheight">�߶�</param>
	/// <param name="Inwidth">���</param>
	/// <param name="Indepth">���</param>
	/// <returns>Mesh����</returns>
	AMesh* CreateBoxMesh(string InName, const float& Inheight, const float& Inwidth, const float& Indepth);
	/// <summary>
	/// ����ƽ��Mesh
	/// </summary>
	/// <param name="InName">����</param>
	/// <param name="Inheight">�߶�</param>
	/// <param name="Inwidth">���</param>
	/// <param name="InHeightSubdivide">�߶�ϸ��</param>
	/// <param name="InwidthSubdivide">���ϸ��</param>
	/// <returns>Mesh����</returns>
	AMesh* CreatePlaneMesh(string InName, const float& Inheight, const float& Inwidth, const uint32_t& InHeightSubdivide, const uint32_t& InwidthSubdivide);
	/// <summary>
	/// ��������Mesh
	/// </summary>
	/// <param name="InName">����</param>
	/// <param name="InTopRadius">���뾶</param>
	/// <param name="InBottomRadius">�װ뾶</param>
	/// <param name="InAxialSubdivision">����ϸ��</param>
	/// <param name="InHeight">�߶�</param>
	/// <param name="InHeightSubdivision">�߶�ϸ��</param>
	/// <returns>Mesh����</returns>
	AMesh* CreateCylinderMesh(string InName, const float& InTopRadius, const float& InBottomRadius, const uint32_t& InAxialSubdivision, const float& InHeight, const uint32_t& InHeightSubdivision);
	/// <summary>
	/// ����OBJMesh
	/// </summary>
	/// <param name="InName">����</param>
	/// <param name="InPath">ģ��·��</param>
	/// <returns>Mesh����</returns>
	AMesh* CreateCustomMesh(string InName, const string& InPath);
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="InName">����</param>
	/// <param name="InRadius">�뾶</param>
	/// <param name="InAxialSubdivision">����ϸ��</param>
	/// <param name="InHeightSubdivision">�߶�ϸ��</param>
	/// <returns>Mesh����</returns>
	AMesh* CreateSphereMesh(string InName, const float& InRadius, const uint32_t& InAxialSubdivision, const uint32_t& InHeightSubdivision);
private:
	template<typename T, typename S, typename ...ParamTypes>
	T* CreateMesh(const S& name, ParamTypes&&... Params);
private:
	FRenderingPipeline RenderingPipeline;					//��Ⱦ���߶���




};
//----------ģ��ʵ��-----
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