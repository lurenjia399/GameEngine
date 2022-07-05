#include "RenderingPipeline.h"

enum class ERenderingPiepelineState;

FRenderingPipeline::FRenderingPipeline()
{
}

bool FRenderingPipeline::FindMeshRenderingDataByHash(const size_t& InHashKey, FGeometryDescData& OutGeometryDescData, int InRenderingLayer)
{
	return GeometryMap.FindMeshRenderingDataByHash(InHashKey, OutGeometryDescData, InRenderingLayer);
}

void FRenderingPipeline::DuplicateMeshRenderingData(CMeshComponent* InMesh, FGeometryDescData& InGeometryDescData)
{
	GeometryMap.DuplicateMeshRenderingData(InMesh, InGeometryDescData);
}

void FRenderingPipeline::BuildMeshComponent(CMeshComponent* InMesh, const FMeshRenderingData& InRenderingData, const size_t& HashKey)
{
	GeometryMap.BuildMeshDescData(InMesh, InRenderingData, HashKey);
}

void FRenderingPipeline::BuildPipeline()
{
	DirectXPiepelineState.ResetGPSDesc();

	//��ʼ����Ⱦ�㼶
	FRenderLayerManage::GetRenderLayerManage()->Init(&GeometryMap, &DirectXPiepelineState);
	//����Ⱦ�㼶��������
	FRenderLayerManage::GetRenderLayerManage()->sort();

	//������ͼ��Դ
	GeometryMap.LoadTexture();
	//�󶨸�ǩ��
	if (!DirectXRootSignature.Build(GeometryMap.GetDrawTextureObjectCount()))
	{
		Engine_Log_Error("������ǩ��ʧ��");
	}
	DirectXPiepelineState.BindRootSignature(DirectXRootSignature.GetRootSignature());

	//ͨ����Ⱦ�㼶����Ӧ����ɫ��
	FRenderLayerManage::GetRenderLayerManage()->BuildShader();

	//����ģ����Դ�������������������
	GeometryMap.BuildMeshBuffer();
	//�����������ѣ����ڴ�ų����������е���Դ��
	GeometryMap.BuildDescriptorHeap();
	//����ģ�͵ĳ���������
	GeometryMap.BuildMeshConstantBufferView();
	//�������ʵ�shader��Դ������,�ռ����е�matrial����������material�е�shaderindex��������ǳ���������������û�з�����������
	GeometryMap.BuildMaterialShaderResourseView();
	//�����ƹ�ĳ���������
	GeometryMap.BuildLightConstantBufferView();
	//�����ӿڵĳ���������
	GeometryMap.BuildViewportConstantBufferView();
	//������ͼ����ɫ����Դ
	GeometryMap.BuildTextureShaderResource();
	
	//����ʵ��pso
	DirectXPiepelineState.BuildParam(D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE::D3D12_CULL_MODE_BACK);
	DirectXPiepelineState.Build((int)ERenderingPiepelineState::GRAYMODEL);
	//�����߿�pso
	DirectXPiepelineState.SetFillMode(D3D12_FILL_MODE::D3D12_FILL_MODE_WIREFRAME);
	DirectXPiepelineState.Build((int)ERenderingPiepelineState::WIREFRAME);
}

void FRenderingPipeline::UpdateConstantView(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	FRenderLayerManage::GetRenderLayerManage()->UpdateObjectConstantBuffer();
	GeometryMap.UpdateConstantView(DeltaTime, ViewportInfo);
}

void FRenderingPipeline::PreDraw(float DeltaTime)
{
	DirectXPiepelineState.PreDraw(DeltaTime);
	FRenderLayerManage::GetRenderLayerManage()->PreDraw(DeltaTime);
	
}

void FRenderingPipeline::Draw(float DeltaTime)
{
	GeometryMap.PreDraw(DeltaTime);
	DirectXRootSignature.PreDraw(DeltaTime);
	GeometryMap.Draw(DeltaTime);
	FRenderLayerManage::GetRenderLayerManage()->Draw(DeltaTime);//Drawÿ����Ⱦ�㼶�ϵ�mesh
	DirectXPiepelineState.Draw(DeltaTime);//��������keyboard 4 5
}

void FRenderingPipeline::PostDraw(float DeltaTime)
{
	GeometryMap.PostDraw(DeltaTime);
	FRenderLayerManage::GetRenderLayerManage()->PostDraw(DeltaTime);
}
