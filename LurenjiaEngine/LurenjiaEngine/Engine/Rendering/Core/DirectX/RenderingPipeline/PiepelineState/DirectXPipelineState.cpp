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
	//��դ��״̬
	GPSDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//���ģʽ
	GPSDesc.RasterizerState.FillMode = InFillMode;
	//����
	GPSDesc.RasterizerState.CullMode = InCullMode;
	//��������
	GPSDesc.SampleMask = UINT_MAX;
	//��������
	GPSDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//��ȾĿ������
	GPSDesc.NumRenderTargets = 1;
	//���״̬
	GPSDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	//���ģ��
	GPSDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	//��������
	GPSDesc.SampleDesc.Count = GetEngine()->GetRenderingEngine()->GetDXGISampleCount();
	//��������
	GPSDesc.SampleDesc.Quality = GetEngine()->GetRenderingEngine()->GetDXGISampleQuality();
	//rtv��ʽ
	GPSDesc.RTVFormats[0] = GetEngine()->GetRenderingEngine()->GetBackBufferFormat();
	//dsv��ʽ
	GPSDesc.DSVFormat = GetEngine()->GetRenderingEngine()->GetDepthStencilFormat();
}

void FDirectXPiepelineState::Build(int InPipelineType)
{
	if (PSO.find(InPipelineType) == PSO.end())
	{
		PSO.emplace(InPipelineType, ComPtr<ID3D12PipelineState>());
	}
	//��������״̬
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
