#pragma once

#ifndef RenderingTextureResourcesUpdate_H
#define RenderingTextureResourcesUpdate_H

#include "../../Core/Engine.h"
#include "../../Interface/DirectXDeviceInterface.h"

struct FRenderingTexture
{
	FRenderingTexture() : RenderingTextureID(0)
	{
	}
	ComPtr<ID3D12Resource> UploadBuffer;
	ComPtr<ID3D12Resource> Data;
	wstring TextureName;
	wstring TexturePath;
	wstring AssertFilename;//资源路径 Texture"Assert/Texture/Hello"
	wstring SimpleAssertFilename;//简单的资源路径 "Assert/Texture/Hello"
	UINT RenderingTextureID;
};

class FRenderingTextureResourcesUpdate : public enable_shared_from_this<FRenderingTextureResourcesUpdate>, public IDirectXDeviceInterface
{
public:
	FRenderingTextureResourcesUpdate();
	void LoadTexture(const wstring& InTexturePath);
	void BuildTextureShaderResource(ID3D12DescriptorHeap* InHeap, int Offset = 0);
	int GetTextureCount() const;
	int GetTextureIndex(string InKey);
public:
	void BuildParam();//构建SRV描述符
	void Set_SRV_ViewDimension(D3D12_SRV_DIMENSION InDimension);
private:
	void Refresh_SRVDescByTextureDimension(unique_ptr<FRenderingTexture>* InRenderingTexture);
private:
	std::map<wstring, unique_ptr<FRenderingTexture>> TextureMapping;
	D3D12_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc;
};

class FRenderingUAVResourvesUpdate : public enable_shared_from_this<FRenderingUAVResourvesUpdate>, public IDirectXDeviceInterface
{
public:
	FRenderingUAVResourvesUpdate();
	void BuildResource();
	void BuildUnorderAccessView(ID3D12DescriptorHeap* InHeap, int Offset = 0);
	void BuildParam();
private:
	D3D12_UNORDERED_ACCESS_VIEW_DESC UAVDesc;//UAV描述
	ComPtr<ID3D12Resource>	pIBlurMap1;//UAV中的资源
	int	iWidth = 1024;
	int	iHeight = 768;
};

#endif
