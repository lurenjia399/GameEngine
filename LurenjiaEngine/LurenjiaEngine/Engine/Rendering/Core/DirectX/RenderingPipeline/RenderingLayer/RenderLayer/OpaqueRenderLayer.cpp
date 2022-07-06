#include "OpaqueRenderLayer.h"

FOpaqueRenderLayer::FOpaqueRenderLayer()
{
	RenderingLayerPriority = 2000;
}

void FOpaqueRenderLayer::BuildShader()
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

void FOpaqueRenderLayer::BuildPSO()
{
	//构建实体pso
	DirectXPiepelineState->BuildParam(D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE::D3D12_CULL_MODE_BACK);
	DirectXPiepelineState->Build((int)ERenderingPiepelineState::GRAYMODEL);
	//构建线框pso
	DirectXPiepelineState->SetFillMode(D3D12_FILL_MODE::D3D12_FILL_MODE_WIREFRAME);
	DirectXPiepelineState->Build((int)ERenderingPiepelineState::WIREFRAME);
}

int FOpaqueRenderLayer::GetRenderLayerType() const
{
	return 0;
}
