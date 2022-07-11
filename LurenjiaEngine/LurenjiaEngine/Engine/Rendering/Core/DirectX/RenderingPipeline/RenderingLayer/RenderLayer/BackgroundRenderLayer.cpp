#include "BackgroundRenderLayer.h"

FBackgroundRenderLayer::FBackgroundRenderLayer()
{
	RenderingLayerPriority = 100;
}

void FBackgroundRenderLayer::UpdateObjectConstantBuffer()
{
	super::UpdateObjectConstantBuffer();
}

void FBackgroundRenderLayer::PreDraw(float DeltaTime)
{
	super::PreDraw(DeltaTime);
}

void FBackgroundRenderLayer::Draw(float DeltaTime)
{
	DirectXPiepelineState->isTemporaryResetPSO((int)EPiepelineStateType::BACKGROUND);

	super::Draw(DeltaTime);

	//每次渲染完当前层级后，需要还原pso的状态
	RestorePSO();
}

void FBackgroundRenderLayer::PostDraw(float DeltaTime)
{
	super::PostDraw(DeltaTime);
}

void FBackgroundRenderLayer::BuildPSO()
{
	super::BuildPSO();

	DirectXPiepelineState->SetFillMode(D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID);
	DirectXPiepelineState->SetCullMode(D3D12_CULL_MODE::D3D12_CULL_MODE_NONE);
	DirectXPiepelineState->Build((int)EPiepelineStateType::BACKGROUND);
}

void FBackgroundRenderLayer::BuildShader()
{
	//绑定着色器
	//着色器宏，传递给着色器相应的值
	char buffer[10] = { 0 };
	D3D_SHADER_MACRO ShaderMacro[] =
	{
		"MapCount", _itoa(GeometryMap->GetDrawTextureObjectCount(), buffer, 10),
		nullptr, nullptr,
	};
	VertexShader.BuildShader(L"../LurenjiaEngine/Shader/Sky.hlsl", "VertexShaderMain", "vs_5_1", ShaderMacro);
	PixelShader.BuildShader(L"../LurenjiaEngine/Shader/Sky.hlsl", "PixelShaderMain", "ps_5_1", ShaderMacro);
	DirectXPiepelineState->BindShader(&VertexShader, &PixelShader);

	//绑定输入布局
	InputElementDesc = {
		{"POSITION",0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};
	DirectXPiepelineState->BindInputLayout(InputElementDesc.data(), (UINT)InputElementDesc.size());
}

int FBackgroundRenderLayer::GetRenderLayerType() const
{
	return (int)EMeshComponentRenderLayerType::RENDERLAYER_BACKGROUND;
}
