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
	/// ����BoxMesh
	/// </summary>
	/// <param name="InName">����</param>
	/// <param name="Inheight">�߶�</param>
	/// <param name="Inwidth">���</param>
	/// <param name="Indepth">���</param>
	/// <returns>Mesh����</returns>
	CMesh* CreateBoxMesh(string InName, const float& Inheight, const float& Inwidth, const float& Indepth);
	/// <summary>
	/// ����ƽ��Mesh
	/// </summary>
	/// <param name="InName">����</param>
	/// <param name="Inheight">�߶�</param>
	/// <param name="Inwidth">���</param>
	/// <param name="InHeightSubdivide">�߶�ϸ��</param>
	/// <param name="InwidthSubdivide">���ϸ��</param>
	/// <returns>Mesh����</returns>
	CMesh* CreatePlaneMesh(string InName, const float& Inheight, const float& Inwidth, const uint32_t& InHeightSubdivide, const uint32_t& InwidthSubdivide);
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
	CMesh* CreateCylinderMesh(string InName, const float& InTopRadius, const float& InBottomRadius, const uint32_t& InAxialSubdivision, const float& InHeight, const uint32_t& InHeightSubdivision);
	/// <summary>
	/// ����OBJMesh
	/// </summary>
	/// <param name="InName">����</param>
	/// <param name="InPath">ģ��·��</param>
	/// <returns>Mesh����</returns>
	CMesh* CreateCustomMesh(string InName, const string& InPath);
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="InName">����</param>
	/// <param name="InRadius">�뾶</param>
	/// <param name="InAxialSubdivision">����ϸ��</param>
	/// <param name="InHeightSubdivision">�߶�ϸ��</param>
	/// <returns>Mesh����</returns>
	CMesh* CreateSphereMesh(string InName, const float& InRadius, const uint32_t& InAxialSubdivision, const uint32_t& InHeightSubdivision);
private:
	template<typename T, typename S, typename ...ParamTypes>
	T* CreateMesh(const S& name, ParamTypes&&... Params);
private:
	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView();
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView();
private:
	ComPtr<ID3DBlob> CPUVertexBufferPtr;					//CPU���mesh�Ķ��㻺����
	ComPtr<ID3DBlob> CPUIndexBufferPtr;						//CPU���mesh������������

	ComPtr<ID3D12Resource> GPUVertexBufferPtr;				//GPUmesh�Ķ��㻺����
	ComPtr<ID3D12Resource> GPUIndexBufferPtr;				//GPUmesh������������

	ComPtr<ID3D12Resource> VertexBufferTempPtr;				//��ʱmesh�Ķ��㻺����
	ComPtr<ID3D12Resource> IndexBufferTempPtr;				//��ʱmesh������������


	ComPtr<ID3D12DescriptorHeap> CBVHeap;					//����������
	shared_ptr<FRenderingResourcesUpdate> objectConstants;	//����ĳ���������
	shared_ptr<FRenderingResourcesUpdate> viewportConstants;	//viewport�ĳ���������

	ComPtr<ID3D12RootSignature> RootSignature;				//��ǩ��

	FShader VertexShader;									//������ɫ��
	FShader PixelShader;									//ƬԪ��ɫ��
	vector<D3D12_INPUT_ELEMENT_DESC> InputElementDesc;		//��ɫ�����벼��

	ComPtr<ID3D12PipelineState> PSO;						//����״̬

private:
	UINT VertexSizeInBytes;									//������Դ��ռ�ڴ��С
	UINT VertexStrideInBytes;								//���㲽����С��Ҳ����ÿ���������ݴ�С��
	UINT IndexSizeInBytes;									//������Դ��ռ�ڴ��С
private:
	DXGI_FORMAT IndexFormat;								//������Դ��ʽ
	UINT IndexSize;											//���������

	XMFLOAT4X4 WorldMatrix;									//mvp�任����
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

	BuildMesh(&MeshRenderingData);

	mesh->Init();
	return mesh;
}