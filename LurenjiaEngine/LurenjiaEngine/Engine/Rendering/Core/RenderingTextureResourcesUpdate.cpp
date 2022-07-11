
#include "RenderingTextureResourcesUpdate.h"

#include "RenderingResourcesUpdate.h"


const wchar_t DDS[] = L".dds";
const wchar_t prefixPath[] = L"../LurenjiaEngine/";

FRenderingTextureResourcesUpdate::FRenderingTextureResourcesUpdate()
{
	memset(&ShaderResourceViewDesc, 0, sizeof(D3D12_SHADER_RESOURCE_VIEW_DESC));
	BuildParam();
}

void FRenderingTextureResourcesUpdate::LoadTexture(const wstring& InTexturePath)
{
	std::unique_ptr<FRenderingTexture> Texture = std::make_unique<FRenderingTexture>();
	Texture->TexturePath = InTexturePath;//�洢��ͼ������·��
	
	wchar_t TextureName[1024] = { '\0'};
	get_path_clean_filename_w(TextureName, InTexturePath.c_str());
	wremove_string_start(TextureName, DDS);
	Texture->TextureName = TextureName;//�洢��ͼ������

	wchar_t* SimpleFilename = const_cast<wchar_t*>(InTexturePath.c_str()) ;
	wremove_string_start(SimpleFilename, prefixPath);
	Texture->SimpleAssertFilename = SimpleFilename;//�洢��ͼ�ļ���Դ·��

	Texture->AssertFilename = L"Texture/" + wstring(SimpleFilename);//�洢��ͼ����Դ·��

	//��ȡdds����
	ANALYSIS_HRESULT(DirectX::CreateDDSTextureFromFile12(
		GetD3dDevice().Get(),
		GetGraphicsCommandList().Get(),
		Texture->TexturePath.c_str(),
		Texture->Data,
		Texture->UploadBuffer));

	Texture->RenderingTextureID = TextureMapping.size();
	//ע������ʹ��std::move
	//unique_ptr����ָ����ָ��ֵ���޷�������
	TextureMapping.emplace(TextureName, std::move(Texture));
}

void FRenderingTextureResourcesUpdate::BuildTextureShaderResource(ID3D12DescriptorHeap* InHeap, int Offset)
{
	UINT HandleSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	CD3DX12_CPU_DESCRIPTOR_HANDLE Handle(InHeap->GetCPUDescriptorHandleForHeapStart());
	Handle.Offset(Offset, HandleSize);

	for (auto& tem : TextureMapping)
	{
		Refresh_SRVDescByTextureDimension(&tem.second);
		GetD3dDevice()->CreateShaderResourceView(tem.second->Data.Get(), &ShaderResourceViewDesc, Handle);
		Handle.Offset(1, HandleSize);
	}
}

int FRenderingTextureResourcesUpdate::GetTextureCount() const
{
	return TextureMapping.size();
}

int FRenderingTextureResourcesUpdate::GetTextureIndex(string InKey)
{
	
	/*
	* const char* inString = InKey.c_str();
	* wchar_t texturePath[1024] = { 0 };
	* char_to_wchar_t(texturePath, 1024, inString);
	*/

	if (InKey == "") return -1;

	wstring key = wstring(InKey.begin(), InKey.end());
	if (TextureMapping.find(key) != TextureMapping.end())
	{
		return TextureMapping[key]->RenderingTextureID;
	}
	else {
		for (auto& tem : TextureMapping)
		{
			if (tem.second->AssertFilename == key)
			{
				return tem.second->RenderingTextureID;
			}
			if (tem.second->SimpleAssertFilename == key)
			{
				return tem.second->RenderingTextureID;
			}
			if (tem.second->TexturePath == key)
			{
				return tem.second->RenderingTextureID;
			}
		}
	}
	Engine_Log_Warning("No Find Texture Resourse [%s] !!! ", InKey);
	return -1;
}

void FRenderingTextureResourcesUpdate::BuildParam()
{
	ShaderResourceViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	ShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	ShaderResourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;//������ɫ���е�float4����ɫͨ��
}

void FRenderingTextureResourcesUpdate::Refresh_SRVDescByTextureDimension(unique_ptr<FRenderingTexture>* InRenderingTexture)
{
	ShaderResourceViewDesc.Format = (*InRenderingTexture)->Data->GetDesc().Format;

	switch (ShaderResourceViewDesc.ViewDimension)
	{
	case D3D12_SRV_DIMENSION_TEXTURE2D :
		ShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		ShaderResourceViewDesc.Texture2D.MipLevels = (*InRenderingTexture)->Data->GetDesc().MipLevels;
		ShaderResourceViewDesc.Texture2D.ResourceMinLODClamp = 0.f;
		ShaderResourceViewDesc.Texture2D.PlaneSlice = 0;
		break;
	case D3D12_SRV_DIMENSION_TEXTURECUBE:
		ShaderResourceViewDesc.TextureCube.MostDetailedMip = 0;
		ShaderResourceViewDesc.TextureCube.MipLevels = (*InRenderingTexture)->Data->GetDesc().MipLevels;
		ShaderResourceViewDesc.TextureCube.ResourceMinLODClamp = 0.f;
		break;
	default:
		break;
	}
}

void FRenderingTextureResourcesUpdate::Set_SRV_ViewDimension(D3D12_SRV_DIMENSION InDimension)
{
	ShaderResourceViewDesc.ViewDimension = InDimension;
}
