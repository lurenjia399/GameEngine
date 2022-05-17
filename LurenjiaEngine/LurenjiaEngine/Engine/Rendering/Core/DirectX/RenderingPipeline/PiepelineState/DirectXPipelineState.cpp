#include "DirectXPipelineState.h"
#include "../../../../../Platform/Windows/WindowsEngine.h"
#include "../../../../Engine/DirectX/Core/DirectXRenderingEngine.h"


FDirectXPiepelineState::FDirectXPiepelineState()
{
	PSO.emplace(ERenderingPiepelineState::WIREFRAME, ComPtr<ID3D12PipelineState>());//�߿�pso
	PSO.emplace(ERenderingPiepelineState::GRAYMODEL, ComPtr<ID3D12PipelineState>());//ģ��pso
	CurrPipelineType = ERenderingPiepelineState::GRAYMODEL;
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

void FDirectXPiepelineState::Build()
{
	//��դ��״̬
	GPSDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	GPSDesc.RasterizerState.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_WIREFRAME;
	GPSDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
	//GPSDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_NONE;
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
	//��������״̬
	ANALYSIS_HRESULT(GetD3dDevice()->CreateGraphicsPipelineState(&GPSDesc, IID_PPV_ARGS(&PSO[static_cast<int>(ERenderingPiepelineState::WIREFRAME)])));
	
	GPSDesc.RasterizerState.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
	ANALYSIS_HRESULT(GetD3dDevice()->CreateGraphicsPipelineState(&GPSDesc, IID_PPV_ARGS(&PSO[static_cast<int>(ERenderingPiepelineState::GRAYMODEL)])));
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
