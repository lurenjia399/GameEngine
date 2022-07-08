#include "DirectXPipelineState.h"
#include "../../../../../Platform/Windows/WindowsEngine.h"
#include "../../../../Engine/DirectX/Core/DirectXRenderingEngine.h"


FDirectXPiepelineState::FDirectXPiepelineState()
	: CurrPipelineType(ERenderingPiepelineState::GRAYMODEL)
{
	GPSDesc = {};
}

void FDirectXPiepelineState::ResetGPSDesc()
{
	memset(&GPSDesc, 0, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
}

void FDirectXPiepelineState::BindInputLayout(const D3D12_INPUT_ELEMENT_DESC* InInputElementDescs, UINT InSize)
{
	GPSDesc.InputLayout.pInputElementDescs = InInputElementDescs;
	GPSDesc.InputLayout.NumElements = InSize;
}

void FDirectXPiepelineState::BindRootSignature(ID3D12RootSignature* RootSignature)
{
	GPSDesc.pRootSignature = RootSignature;
}

void FDirectXPiepelineState::BindShader(const FShader* VertexShader, const FShader* PixelShader)
{
	GPSDesc.VS.pShaderBytecode = reinterpret_cast<BYTE*>(VertexShader->GetBufferPointer());
	GPSDesc.VS.BytecodeLength = VertexShader->GetBufferSize();

	GPSDesc.PS.pShaderBytecode = PixelShader->GetBufferPointer();
	GPSDesc.PS.BytecodeLength = PixelShader->GetBufferSize();
}

void FDirectXPiepelineState::BuildParam(D3D12_FILL_MODE InFillMode, D3D12_CULL_MODE InCullMode)
{
	//光栅化状态
	GPSDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//填充模式
	GPSDesc.RasterizerState.FillMode = InFillMode;
	//剪裁
	GPSDesc.RasterizerState.CullMode = InCullMode;
	//采样掩码
	GPSDesc.SampleMask = UINT_MAX;
	//拓扑类型
	GPSDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//渲染目标数量
	GPSDesc.NumRenderTargets = 1;
	//混合状态
	GPSDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	//深度模板
	GPSDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	//采样数量
	GPSDesc.SampleDesc.Count = GetEngine()->GetRenderingEngine()->GetDXGISampleCount();
	//采样质量
	GPSDesc.SampleDesc.Quality = GetEngine()->GetRenderingEngine()->GetDXGISampleQuality();
	//rtv格式
	GPSDesc.RTVFormats[0] = GetEngine()->GetRenderingEngine()->GetBackBufferFormat();
	//dsv格式
	GPSDesc.DSVFormat = GetEngine()->GetRenderingEngine()->GetDepthStencilFormat();
}

void FDirectXPiepelineState::Build(int InPipelineType)
{
	if (PSO.find(InPipelineType) == PSO.end())
	{
		PSO.emplace(InPipelineType, ComPtr<ID3D12PipelineState>());
	}
	//创建管线状态
	ANALYSIS_HRESULT(GetD3dDevice()->CreateGraphicsPipelineState(&GPSDesc, IID_PPV_ARGS(&PSO[InPipelineType])));
}

void FDirectXPiepelineState::ResetPSO(int InPiepelType)
{
	GetGraphicsCommandList()->SetPipelineState(PSO[InPiepelType].Get());
}

void FDirectXPiepelineState::PreDraw(float DeltaTime)
{
	GetGraphicsCommandList()->Reset(GetCommandAllocator().Get(), PSO[static_cast<int>(CurrPipelineType)].Get());
}

void FDirectXPiepelineState::Draw(float DeltaTime)
{
	CaptureKeyboardKeys();
}

void FDirectXPiepelineState::PostDraw(float DeltaTime)
{
}

void FDirectXPiepelineState::SetFillMode(D3D12_FILL_MODE InFillMode)
{
	GPSDesc.RasterizerState.FillMode = InFillMode;
}

void FDirectXPiepelineState::SetCullMode(D3D12_CULL_MODE InCullMode)
{
	GPSDesc.RasterizerState.CullMode = InCullMode;
}

void FDirectXPiepelineState::SetRenderingTarget(int Index, const D3D12_RENDER_TARGET_BLEND_DESC& InRenderTargetBlendDesc)
{
	GPSDesc.BlendState.RenderTarget[Index] = InRenderTargetBlendDesc;
}

void FDirectXPiepelineState::CaptureKeyboardKeys()
{
	if ((GetAsyncKeyState('4') & 0x8000) && CurrPipelineType != ERenderingPiepelineState::WIREFRAME)
	{
		CurrPipelineType = ERenderingPiepelineState::WIREFRAME;
	}
	else if ((GetAsyncKeyState('5') & 0x8000) && CurrPipelineType != ERenderingPiepelineState::GRAYMODEL)
	{
		CurrPipelineType = ERenderingPiepelineState::GRAYMODEL;
	}
}
