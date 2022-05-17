#include "RenderingPipeline.h"

FRenderingPipeline::FRenderingPipeline()
{
}

void FRenderingPipeline::BuildMesh(AMesh* InMesh, const FMeshRenderingData& InRenderingData)
{
	GeometryMap.BuildMeshDescData(InMesh, InRenderingData);
}

void FRenderingPipeline::BuildPipeline()
{
	DirectXPiepelineState.ResetGPSDesc();
	//�󶨸�ǩ��
	if (!DirectXRootSignature.Build())
	{
		Engine_Log_Error("������ǩ��ʧ��");
	}
	DirectXPiepelineState.BindRootSignature(DirectXRootSignature.GetRootSignature());
	//����ɫ��
	VertexShader.BuildShader(L"../LurenjiaEngine/Shader/main.hlsl", "VertexShaderMain", "vs_5_0");
	PixelShader.BuildShader(L"../LurenjiaEngine/Shader/main.hlsl", "PixelShaderMain", "ps_5_0");
	DirectXPiepelineState.BindShader(&VertexShader, &PixelShader);
	//�������������
	InputElementDesc = {
		{"POSITION",0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};
	DirectXPiepelineState.BindInputLayout(InputElementDesc.data(), (UINT)InputElementDesc.size());
	//����ģ����Դ�������������������
	GeometryMap.BuildMeshBuffer();
	//�����������ѣ����ڴ�ų���������������
	GeometryMap.BuildDescriptorHeap();
	//����ģ�͵ĳ���������
	GeometryMap.BuildMeshConstantBufferView();
	//�������ʵĳ���������
	GeometryMap.BuildMaterialConstantBufferView();
	//�����ƹ�ĳ���������
	GeometryMap.BuildLightConstantBufferView();
	//�����ӿڵĳ���������
	GeometryMap.BuildViewportConstantBufferView();
	
	DirectXPiepelineState.Build();
}

void FRenderingPipeline::UpdateConstantView(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	GeometryMap.UpdateConstantView(DeltaTime, ViewportInfo);
}

void FRenderingPipeline::PreDraw(float DeltaTime)
{
	DirectXPiepelineState.PreDraw(DeltaTime);
	
}

void FRenderingPipeline::Draw(float DeltaTime)
{
	GeometryMap.PreDraw(DeltaTime);
	DirectXRootSignature.PreDraw(DeltaTime);
	GeometryMap.Draw(DeltaTime);
	DirectXPiepelineState.Draw(DeltaTime);//��������keyboard 4 5
}

void FRenderingPipeline::PostDraw(float DeltaTime)
{
	GeometryMap.PostDraw(DeltaTime);
}
