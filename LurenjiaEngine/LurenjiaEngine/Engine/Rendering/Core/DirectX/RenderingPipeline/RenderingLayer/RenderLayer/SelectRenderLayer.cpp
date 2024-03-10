#include "SelectRenderLayer.h"

FSelectRenderLayer::FSelectRenderLayer()
{
	RenderingLayerPriority = 3100;
}

void FSelectRenderLayer::BuildShader()
{
	vector<ShaderType::FShaderMacro> SelfShaderMacro;
	BuildShaderMacro(SelfShaderMacro);
	std::vector<D3D_SHADER_MACRO> ShaderMacro;
	bool isSuccess = ShaderType::TransD3dShaderMacro(SelfShaderMacro, ShaderMacro);
	assert(isSuccess);
	VertexShader.BuildShader(L"../LurenjiaEngine/Shader/SelectActor.hlsl", "VertexShaderMain", "vs_5_1", ShaderMacro.data());
	PixelShader.BuildShader(L"../LurenjiaEngine/Shader/SelectActor.hlsl", "PixelShaderMain", "ps_5_1", ShaderMacro.data());
	DirectXPiepelineState->BindShader(&VertexShader, &PixelShader);

	//绑定输入布局
	InputElementDesc = {
		{"POSITION",0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};
	DirectXPiepelineState->BindInputLayout(InputElementDesc.data(), (UINT)InputElementDesc.size());
}

void FSelectRenderLayer::BuildPSO()
{
	Super::BuildPSO();

	DirectXPiepelineState->Build((int)EPiepelineStateType::SELECT);
}

void FSelectRenderLayer::PreDraw(float DeltaTime)
{
	Super::PreDraw(DeltaTime);
}

void FSelectRenderLayer::Draw(float DeltaTime)
{
	DirectXPiepelineState->isTemporaryResetPSO((int)EPiepelineStateType::SELECT);

	Super::Draw(DeltaTime);

	//每次渲染完当前层级后，需要还原pso的状态
	RestorePSO();
}

void FSelectRenderLayer::PostDraw(float DeltaTime)
{
	Super::PostDraw(DeltaTime);
}

int FSelectRenderLayer::GetRenderLayerType() const
{
	return (int)EMeshComponentRenderLayerType::RENDERLAYER_SELECT;
}
