#include "RenderingPipeline.h"

enum class EPiepelineStateType;

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

	//����ģ����Դ�������������������
	GeometryMap.BuildMeshBuffer();

	//������ĳ���������
	GeometryMap.BuildFogConstantBufferView();
	//����ģ�͵ĳ���������
	GeometryMap.BuildMeshConstantBufferView();
	//�������ʵ�shader��Դ������,�ռ����е�matrial����������material�е�shaderindex��������ǳ���������������û�з�����������
	GeometryMap.BuildMaterialShaderResourseView();
	//�����ƹ�ĳ���������
	GeometryMap.BuildLightConstantBufferView();
	//�����ӿڵĳ���������
	GeometryMap.BuildViewportConstantBufferView();

	//�����������ѣ����ڴ�ų����������е���Դ��, ������ȫ����ͼ��Դ
	GeometryMap.BuildDescriptorHeap();
	//������ͼ����ɫ����Դ
	GeometryMap.BuildTextureShaderResource();

	//����pso
	FRenderLayerManage::GetRenderLayerManage()->BuildPSO();
}

void FRenderingPipeline::UpdateConstantView(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	//����ÿ���㼶��ģ�͵ĳ�����������Դ
	FRenderLayerManage::GetRenderLayerManage()->UpdateObjectConstantBuffer();
	//���²��ʳ���������
	GeometryMap.UpdateMaterialShaderResourceView(DeltaTime, ViewportInfo);
	//���µƹ�ĳ���������
	GeometryMap.UpdateLightConstantBufferView(DeltaTime, ViewportInfo);
	//�����ӿڵĳ���������
	GeometryMap.UpdateViewportConstantBufferView(DeltaTime, ViewportInfo);
	//���������ĳ���������
	GeometryMap.UpdateFogConstantBufferView(DeltaTime, ViewportInfo);
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
