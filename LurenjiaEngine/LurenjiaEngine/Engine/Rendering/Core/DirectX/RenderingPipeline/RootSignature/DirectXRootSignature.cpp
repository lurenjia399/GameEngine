#include "DirectXRootSignature.h"

FDirectXRootSignature::FDirectXRootSignature()
{
}

bool FDirectXRootSignature::Build(const UINT& TextureCount)
{
	CD3DX12_ROOT_PARAMETER RootParam[7];

	//CubeMap的descriptorRange
	CD3DX12_DESCRIPTOR_RANGE DescriptorRangeCubeMapSRV = {};
	DescriptorRangeCubeMapSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	//Texture的descriptorRange
	CD3DX12_DESCRIPTOR_RANGE DescriptorRangeTextureSRV = {};
	DescriptorRangeTextureSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, TextureCount, 1);

	RootParam[0].InitAsConstantBufferView(0);//meshobject
	RootParam[1].InitAsConstantBufferView(1);//viewport
	RootParam[2].InitAsConstantBufferView(2);//light
	RootParam[3].InitAsConstantBufferView(3);//fog
	RootParam[4].InitAsShaderResourceView(0, 1);//material

	RootParam[5].InitAsDescriptorTable(1, &DescriptorRangeTextureSRV, D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_PIXEL);//texture
	RootParam[6].InitAsDescriptorTable(1, &DescriptorRangeCubeMapSRV, D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_PIXEL);//cubemap

	//构建静态采样
	StaticSamplerObject.BuildStaticSampler();

	CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc(
		7, RootParam, StaticSamplerObject.GetSize(), StaticSamplerObject.GetData(),
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
	);
	ComPtr<ID3DBlob> SerializeRootSignature;
	ComPtr<ID3DBlob> errorSerializeRootSignature;
	D3D12SerializeRootSignature(
		&RootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1,
		SerializeRootSignature.GetAddressOf(),
		errorSerializeRootSignature.GetAddressOf());
	if (errorSerializeRootSignature != nullptr)
	{
		Engine_Log_Error("root signature error = %s", (char*)errorSerializeRootSignature->GetBufferPointer());
		return false;
	}
	ANALYSIS_HRESULT(GetD3dDevice()->CreateRootSignature(
		0,
		SerializeRootSignature->GetBufferPointer(),
		SerializeRootSignature->GetBufferSize(),
		IID_PPV_ARGS(&RootSignature)));
	return true;
}

ID3D12RootSignature* FDirectXRootSignature::GetRootSignature()
{
	return RootSignature.Get();
}

void FDirectXRootSignature::PreDraw(float DeltaTime)
{
	//向命令列表中 添加根签名 命令
	GetGraphicsCommandList()->SetGraphicsRootSignature(RootSignature.Get());
}

void FDirectXRootSignature::Draw(float DeltaTime)
{
}

void FDirectXRootSignature::PostDraw(float DeltaTime)
{
}
