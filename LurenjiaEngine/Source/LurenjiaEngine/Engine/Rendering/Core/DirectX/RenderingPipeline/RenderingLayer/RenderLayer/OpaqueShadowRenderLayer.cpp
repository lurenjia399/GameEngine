#include "OpaqueShadowRenderLayer.h"
#include "../RenderLayerManage.h"

FOpaqueShadowRenderLayer::FOpaqueShadowRenderLayer()
{
	RenderingLayerPriority = 1280;
}

void FOpaqueShadowRenderLayer::BuildShader()
{
	//绑定着色器
	//着色器宏，传递给着色器相应的值
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

	//绑定输入布局
	InputElementDesc = {
		{"POSITION",0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};
	DirectXPiepelineState->BindInputLayout(InputElementDesc.data(), (UINT)InputElementDesc.size());
}

void FOpaqueShadowRenderLayer::BuildPSO()
{
	super::BuildPSO();

	// 偏移补偿，用来解决shadowmap自遮挡问题
	// Bias = (float)DepthBias * r + SlopeScaledDepthBias * MaxDepthSlope
	// where r is the minimum representable value > 0 in the depth-buffer format converted to float32
	// The MaxDepthSlope value is the maximum of the horizontal and vertical slopes of the depth value at the pixel
	D3D12_GRAPHICS_PIPELINE_STATE_DESC& PSODesc = DirectXPiepelineState->GetPSODesc();
	// 固定的应用偏移量
	PSODesc.RasterizerState.DepthBias = 100000;
	// 所允许的最大深度偏移量
	PSODesc.RasterizerState.DepthBiasClamp = 0.0f;
	// 根据多边形的斜率控制偏移程度的缩放因子
	PSODesc.RasterizerState.SlopeScaledDepthBias = 1.0f;

	// 关掉rtv
	PSODesc.RTVFormats[0] = DXGI_FORMAT_UNKNOWN;
	PSODesc.NumRenderTargets = 0;

	//构建阴影pso
	DirectXPiepelineState->Build((int)EPiepelineStateType::SHADOW);


}

void FOpaqueShadowRenderLayer::Draw(float DeltaTime)
{
	// 阴影在渲染的时候，首先设置自己的pso
	// 然后开始渲染mesh，
	// 还原pso
	DirectXPiepelineState->isTemporaryResetPSO((int)EPiepelineStateType::SHADOW, true);

	FRenderLayerManage::GetRenderLayerManage()->DrawAllObjectsByLayer(DeltaTime, (int)EMeshComponentRenderLayerType::RENDERLAYER_OPAQUE);
	FRenderLayerManage::GetRenderLayerManage()->DrawAllObjectsByLayer(DeltaTime, (int)EMeshComponentRenderLayerType::RENDERLAYER_TRANSPARENT);
	FRenderLayerManage::GetRenderLayerManage()->DrawAllObjectsByLayer(DeltaTime, (int)EMeshComponentRenderLayerType::RENDERLAYER_OPAQUEREFLECT);

	//每次渲染完当前层级后，需要还原pso的状态
	RestorePSO();

}

int FOpaqueShadowRenderLayer::GetRenderLayerType() const
{
	return (int)EMeshComponentRenderLayerType::RENDERLAYER_OPAQUESHADOW;
}

void FOpaqueShadowRenderLayer::ResetPSO()
{
	// 这边我写的不一样，之后再看
}
