#include "TransparentRenderLayer.h"

FTransparentRenderLayer::FTransparentRenderLayer()
{
	RenderingLayerPriority = 3000;
}

void FTransparentRenderLayer::BuildPSO()
{
	super::BuildPSO();

	D3D12_RENDER_TARGET_BLEND_DESC RenderTargetBlendDesc = {};
	RenderTargetBlendDesc.BlendEnable = true;
	RenderTargetBlendDesc.LogicOpEnable = false;

	RenderTargetBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	RenderTargetBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	RenderTargetBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;

	RenderTargetBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;//F(1,1,1)
	RenderTargetBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;//F(0,0,0)
	RenderTargetBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;

	RenderTargetBlendDesc.LogicOp = D3D12_LOGIC_OP_NOOP;
	RenderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	DirectXPiepelineState->SetRenderingTarget(0, RenderTargetBlendDesc);
	DirectXPiepelineState->Build((int)EPiepelineStateType::TRANSPARENTS);
}

void FTransparentRenderLayer::BuildShader()
{
	//绑定着色器
	//着色器宏，传递给着色器相应的值
	char buffer[10] = { 0 };
	D3D_SHADER_MACRO ShaderMacro[] =
	{
		"MapCount", _itoa(GeometryMap->GetDrawTextureObjectCount(), buffer, 10),
		nullptr, nullptr,
	};
	VertexShader.BuildShader(L"../LurenjiaEngine/Shader/main.hlsl", "VertexShaderMain", "vs_5_1", ShaderMacro);
	PixelShader.BuildShader(L"../LurenjiaEngine/Shader/main.hlsl", "PixelShaderMain", "ps_5_1", ShaderMacro);
	DirectXPiepelineState->BindShader(&VertexShader, &PixelShader);

	//绑定输入布局
	InputElementDesc = {
		{"POSITION",0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"UTANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 40, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 52, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};
	DirectXPiepelineState->BindInputLayout(InputElementDesc.data(), (UINT)InputElementDesc.size());
}

void FTransparentRenderLayer::Draw(float DeltaTime)
{
	DirectXPiepelineState->isTemporaryResetPSO((int)EPiepelineStateType::TRANSPARENTS);

	super::Draw(DeltaTime);

	//每次渲染完当前层级后，需要还原pso的状态
	RestorePSO();
}

int FTransparentRenderLayer::GetRenderLayerType() const
{
	return (int)EMeshComponentRenderLayerType::RENDERLAYER_TRANSPARENT;
}
