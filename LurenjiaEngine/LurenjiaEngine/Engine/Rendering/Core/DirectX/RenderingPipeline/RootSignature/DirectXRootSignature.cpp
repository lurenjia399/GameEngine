#include "DirectXRootSignature.h"

FDirectXRootSignature::FDirectXRootSignature()
{
}

bool FDirectXRootSignature::Build()
{
	CD3DX12_ROOT_PARAMETER RootParam[5];
	//����ĵ�descriptorRange
	CD3DX12_DESCRIPTOR_RANGE DescriptorRangeObjCBV;
	DescriptorRangeObjCBV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);

	//viewport��descriptorRange
	CD3DX12_DESCRIPTOR_RANGE DescriptorRangeViewportCBV;
	DescriptorRangeViewportCBV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);

	//material��descriptorRange
	CD3DX12_DESCRIPTOR_RANGE DescriptorRangeMaterialCBV;
	DescriptorRangeMaterialCBV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 2);

	//�ƹ��desctriptorRange
	CD3DX12_DESCRIPTOR_RANGE DescriptorRangeLightCBV;
	DescriptorRangeLightCBV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 3);

	CD3DX12_DESCRIPTOR_RANGE DescriptorRangeTextureSRV;
	DescriptorRangeTextureSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 4);

	RootParam[0].InitAsDescriptorTable(1, &DescriptorRangeObjCBV);
	RootParam[1].InitAsDescriptorTable(1, &DescriptorRangeViewportCBV);
	RootParam[2].InitAsDescriptorTable(1, &DescriptorRangeMaterialCBV);
	RootParam[3].InitAsDescriptorTable(1, &DescriptorRangeLightCBV);
	RootParam[4].InitAsDescriptorTable(1, &DescriptorRangeTextureSRV, D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_PIXEL);

	//��̬������ʽ
	vector<D3D12_STATIC_SAMPLER_DESC> SamplerDesc;
	SamplerDesc.emplace_back(CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER::D3D12_FILTER_MIN_MAG_MIP_POINT
		, D3D12_TEXTURE_ADDRESS_MODE_MIRROR, D3D12_TEXTURE_ADDRESS_MODE_MIRROR));

	CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc(
		5, RootParam, SamplerDesc.size(), SamplerDesc.data(),
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
	//�������б��� ��Ӹ�ǩ�� ����
	GetGraphicsCommandList()->SetGraphicsRootSignature(RootSignature.Get());
}

void FDirectXRootSignature::Draw(float DeltaTime)
{
}

void FDirectXRootSignature::PostDraw(float DeltaTime)
{
}
