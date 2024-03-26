#include "OpaqueReflectorRenderLayer.h"

FOpaqueReflectorRenderLayer::FOpaqueReflectorRenderLayer()
{
	RenderingLayerPriority = 2100;
}

void FOpaqueReflectorRenderLayer::BuildShader()
{

	//����ɫ��
	//��ɫ���꣬���ݸ���ɫ����Ӧ��ֵ
	vector<ShaderType::FShaderMacro> SelfShaderMacro;
	BuildShaderMacro(SelfShaderMacro);
	std::vector<D3D_SHADER_MACRO> ShaderMacro;
	bool isSuccess = ShaderType::TransD3dShaderMacro(SelfShaderMacro, ShaderMacro);
	assert(isSuccess);
	VertexShader.BuildShader(L"../LurenjiaEngine/Shader/main.hlsl", "VertexShaderMain", "vs_5_1", ShaderMacro.data());
	PixelShader.BuildShader(L"../LurenjiaEngine/Shader/main.hlsl", "PixelShaderMain", "ps_5_1", ShaderMacro.data());
	DirectXPiepelineState->BindShader(&VertexShader, &PixelShader);

	//�����벼��
	InputElementDesc = {
		{"POSITION",0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"UTANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 40, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 52, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};
	DirectXPiepelineState->BindInputLayout(InputElementDesc.data(), (UINT)InputElementDesc.size());
}

void FOpaqueReflectorRenderLayer::BuildPSO()
{
	super::BuildPSO();

	//����ʵ��pso
	DirectXPiepelineState->Build((int)EPiepelineStateType::REFLECTION);
	//�����߿�pso
	/*DirectXPiepelineState->SetFillMode(D3D12_FILL_MODE::D3D12_FILL_MODE_WIREFRAME);
	DirectXPiepelineState->Build((int)EPiepelineStateType::WIREFRAME);*/
}

void FOpaqueReflectorRenderLayer::Draw(float DeltaTime)
{
	super::Draw(DeltaTime);
}

int FOpaqueReflectorRenderLayer::GetRenderLayerType() const
{
	return (int)EMeshComponentRenderLayerType::RENDERLAYER_OPAQUEREFLECT;
}
