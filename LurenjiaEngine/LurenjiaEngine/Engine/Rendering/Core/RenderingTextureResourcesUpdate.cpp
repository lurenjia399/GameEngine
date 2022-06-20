
#include "RenderingTextureResourcesUpdate.h"

#include "RenderingResourcesUpdate.h"


const wchar_t DDS[] = L".dds";
const wchar_t prefixPath[] = L"../LurenjiaEngine/";

void FRenderingTextureResourcesUpdate::LoadTexture(const wstring& InTexturePath)
{
	std::unique_ptr<FRenderingTexture> Texture = std::make_unique<FRenderingTexture>();
	Texture->TexturePath = InTexturePath;//存储贴图的完整路径
	
	wchar_t TextureName[1024] = { '\0'};
	get_path_clean_filename_w(TextureName, InTexturePath.c_str());
	wremove_string_start(TextureName, DDS);
	Texture->TextureName = TextureName;//存储贴图的名称

	wchar_t* SimpleFilename = const_cast<wchar_t*>(InTexturePath.c_str()) ;
	wremove_string_start(SimpleFilename, prefixPath);
	Texture->SimpleAssertFilename = SimpleFilename;//存储贴图的简单资源路径

	Texture->AssertFilename = L"Texture/" + wstring(SimpleFilename);//存储贴图的资源路径

	//读取dds数据
	ANALYSIS_HRESULT(DirectX::CreateDDSTextureFromFile12(
		GetD3dDevice().Get(),
		GetGraphicsCommandList().Get(),
		Texture->TexturePath.c_str(),
		Texture->Data,
		Texture->UploadBuffer));

	Texture->RenderingTextureID = TextureMapping.size();
	//注意这里使用std::move
	//unique_ptr智能指针所指的值，无法被拷贝
	TextureMapping.emplace(TextureName, std::move(Texture));
}

void FRenderingTextureResourcesUpdate::BuildTextureShaderResource(ID3D12DescriptorHeap* InHeap, int Offset)
{
	UINT HandleSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	CD3DX12_CPU_DESCRIPTOR_HANDLE Handle(InHeap->GetCPUDescriptorHandleForHeapStart());
	Handle.Offset(Offset, HandleSize);

	D3D12_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc = {};
	ShaderResourceViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	ShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	ShaderResourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;//设置着色器中的float4的颜色通道
	ShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	ShaderResourceViewDesc.Texture2D.MipLevels = 1;
	ShaderResourceViewDesc.Texture2D.ResourceMinLODClamp = 0.f;

	for (auto& tem : TextureMapping)
	{
		ShaderResourceViewDesc.Format = tem.second->Data->GetDesc().Format;
		ShaderResourceViewDesc.Texture2D.MipLevels = tem.second->Data->GetDesc().MipLevels;
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
