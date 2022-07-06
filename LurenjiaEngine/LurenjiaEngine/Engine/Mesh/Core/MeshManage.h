#pragma once
#include "../../Core/CoreObject/CoreMinimalObject.h"
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
	CMeshComponent* CreateBoxMeshComponent(string InName, const float& Inheight, const float& Inwidth, const float& Indepth);
	/// <summary>
	/// ����ƽ��Mesh
	/// </summary>
	/// <param name="InName">����</param>
	/// <param name="Inheight">�߶�</param>
	/// <param name="Inwidth">���</param>
	/// <param name="InHeightSubdivide">�߶�ϸ��</param>
	/// <param name="InwidthSubdivide">���ϸ��</param>
	/// <returns>Mesh����</returns>
	CMeshComponent* CreatePlaneMeshComponent(string InName, const float& Inheight, const float& Inwidth, const uint32_t& InHeightSubdivide, const uint32_t& InwidthSubdivide);
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
	CMeshComponent* CreateCylinderMeshComponent(string InName, const float& InTopRadius, const float& InBottomRadius, const uint32_t& InAxialSubdivision, const float& InHeight, const uint32_t& InHeightSubdivision);
	/// <summary>
	/// ����OBJMesh
	/// </summary>
	/// <param name="InName">����</param>
	/// <param name="InPath">ģ��·��</param>
	/// <returns>Mesh����</returns>
	CMeshComponent* CreateCustomMeshComponent(string InName, const string& InPath);
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="InName">����</param>
	/// <param name="InRadius">�뾶</param>
	/// <param name="InAxialSubdivision">����ϸ��</param>
	/// <param name="InHeightSubdivision">�߶�ϸ��</param>
	/// <returns>Mesh����</returns>
	CMeshComponent* CreateSphereMeshComponent(string InName, const float& InRadius, const uint32_t& InAxialSubdivision, const uint32_t& InHeightSubdivision);
public:
	FRenderingPipeline& GetRenderingPiepeline();
private:
	FRenderingPipeline RenderingPipeline;					//��Ⱦ���߶���
};
//----------ģ��ʵ��-----
template<typename T>
string getName(T name)
{
	return name;
}