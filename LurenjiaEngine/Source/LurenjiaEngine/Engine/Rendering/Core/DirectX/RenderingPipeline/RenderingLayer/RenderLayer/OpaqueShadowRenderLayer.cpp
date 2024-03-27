#include "OpaqueShadowRenderLayer.h"
#include "../RenderLayerManage.h"

FOpaqueShadowRenderLayer::FOpaqueShadowRenderLayer()
{
	RenderingLayerPriority = 1280;
}

void FOpaqueShadowRenderLayer::BuildShader()
{
	//����ɫ��
	//��ɫ���꣬���ݸ���ɫ����Ӧ��ֵ
	vector<ShaderType::FShaderMacro> SelfShaderMacro;
	BuildShaderMacro(SelfShaderMacro);
	std::vector<D3D_SHADER_MACRO> ShaderMacro;
	bool isSuccess = ShaderType::TransD3dShaderMacro(SelfShaderMacro, ShaderMacro);
	assert(isSuccess);
	wstring ShadowPath = L"/Shadow.hlsl";
	wstring ShaderPath = FEnginePathHelper::GetEngineShadersWidthPath();
	VertexShader.BuildShader(ShaderPath + ShadowPath, "VertexShaderMain", "vs_5_1", ShaderMacro.data());
	PixelShader.BuildShader(ShaderPath + ShadowPath, "PixelShaderMain", "ps_5_1", ShaderMacro.data());
	DirectXPiepelineState->BindShader(&VertexShader, &PixelShader);

	//�����벼��
	InputElementDesc = {
		{"POSITION",0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};
	DirectXPiepelineState->BindInputLayout(InputElementDesc.data(), (UINT)InputElementDesc.size());
}

void FOpaqueShadowRenderLayer::BuildPSO()
{
	super::BuildPSO();

	// ƫ�Ʋ������������shadowmap���ڵ�����
	// Bias = (float)DepthBias * r + SlopeScaledDepthBias * MaxDepthSlope
	// where r is the minimum representable value > 0 in the depth-buffer format converted to float32
	// The MaxDepthSlope value is the maximum of the horizontal and vertical slopes of the depth value at the pixel
	D3D12_GRAPHICS_PIPELINE_STATE_DESC& PSODesc = DirectXPiepelineState->GetPSODesc();
	// �̶���Ӧ��ƫ����
	PSODesc.RasterizerState.DepthBias = 100000;
	// �������������ƫ����
	PSODesc.RasterizerState.DepthBiasClamp = 0.0f;
	// ���ݶ���ε�б�ʿ���ƫ�Ƴ̶ȵ���������
	PSODesc.RasterizerState.SlopeScaledDepthBias = 1.0f;

	// �ص�rtv
	PSODesc.RTVFormats[0] = DXGI_FORMAT_UNKNOWN;
	PSODesc.NumRenderTargets = 0;

	//������Ӱpso
	DirectXPiepelineState->Build((int)EPiepelineStateType::SHADOW);


}

void FOpaqueShadowRenderLayer::Draw(float DeltaTime)
{
	// ��Ӱ����Ⱦ��ʱ�����������Լ���pso
	// Ȼ��ʼ��Ⱦmesh��
	// ��ԭpso
	DirectXPiepelineState->isTemporaryResetPSO((int)EPiepelineStateType::SHADOW, true);

	FRenderLayerManage::GetRenderLayerManage()->DrawAllObjectsByLayer(DeltaTime, (int)EMeshComponentRenderLayerType::RENDERLAYER_OPAQUE);
	FRenderLayerManage::GetRenderLayerManage()->DrawAllObjectsByLayer(DeltaTime, (int)EMeshComponentRenderLayerType::RENDERLAYER_TRANSPARENT);
	FRenderLayerManage::GetRenderLayerManage()->DrawAllObjectsByLayer(DeltaTime, (int)EMeshComponentRenderLayerType::RENDERLAYER_OPAQUEREFLECT);

	//ÿ����Ⱦ�굱ǰ�㼶����Ҫ��ԭpso��״̬
	RestorePSO();

}

int FOpaqueShadowRenderLayer::GetRenderLayerType() const
{
	return (int)EMeshComponentRenderLayerType::RENDERLAYER_OPAQUESHADOW;
}

void FOpaqueShadowRenderLayer::ResetPSO()
{
	// �����д�Ĳ�һ����֮���ٿ�
}
