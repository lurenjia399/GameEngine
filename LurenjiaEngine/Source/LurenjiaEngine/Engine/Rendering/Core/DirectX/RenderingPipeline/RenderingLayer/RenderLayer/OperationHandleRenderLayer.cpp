#include "OperationHandleRenderLayer.h"

FOperationHandleRenderLayer::FOperationHandleRenderLayer()
{
	RenderingLayerPriority = 3200;
}

void FOperationHandleRenderLayer::BuildPSO()
{
	super::BuildPSO();

	DirectXPiepelineState->Build((int)EPiepelineStateType::OPERATIONHANDLE);
}

void FOperationHandleRenderLayer::BuildShader()
{
	//����ɫ��
	//��ɫ���꣬���ݸ���ɫ����Ӧ��ֵ
	vector<ShaderType::FShaderMacro> SelfShaderMacro;
	BuildShaderMacro(SelfShaderMacro);
	std::vector<D3D_SHADER_MACRO> ShaderMacro;
	bool isSuccess = ShaderType::TransD3dShaderMacro(SelfShaderMacro, ShaderMacro);
	assert(isSuccess);
	wstring TransparentPath = L"/OperationHandle.hlsl";
	wstring ShaderPath = FEnginePathHelper::GetEngineShadersWidthPath();
	VertexShader.BuildShader(ShaderPath + TransparentPath, "VertexShaderMain", "vs_5_1", ShaderMacro.data());
	PixelShader.BuildShader(ShaderPath + TransparentPath, "PixelShaderMain", "ps_5_1", ShaderMacro.data());
	DirectXPiepelineState->BindShader(&VertexShader, &PixelShader);

	//�����벼��
	InputElementDesc = {
		{"POSITION",0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};
	DirectXPiepelineState->BindInputLayout(InputElementDesc.data(), (UINT)InputElementDesc.size());
}

void FOperationHandleRenderLayer::Draw(float DeltaTime)
{
	// ���ÿ����draw��ʱ����Ҫ�л�pso��
	// ����ط�û��ע��������������Ӱ�Ƿ���Ҫ�л���
	DirectXPiepelineState->isTemporaryResetPSO((int)EPiepelineStateType::OPERATIONHANDLE);

	super::Draw(DeltaTime);

	RestorePSO();
}

int FOperationHandleRenderLayer::GetRenderLayerType() const
{
	return (int)EMeshComponentRenderLayerType::RENDERLAYER_OPERATIONHANDLE;
}
