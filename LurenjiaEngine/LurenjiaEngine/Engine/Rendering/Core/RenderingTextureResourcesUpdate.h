#pragma once

#ifndef RenderingTextureResourcesUpdate_H
#define RenderingTextureResourcesUpdate_H

#include "../../Core/Engine.h"
#include "../../Interface/DirectXDeviceInterface.h"

struct FRenderingTexture
{
	ComPtr<ID3D12Resource> UploadBuffer;
	ComPtr<ID3D12Resource> Data;
	wstring TextureName;
	wstring TexturePath;
};

class FRenderingTextureResourcesUpdate : public enable_shared_from_this<FRenderingTextureResourcesUpdate>, public IDirectXDeviceInterface
{
public:
	void LoadTexture(const wstring& InTexturePath);
	void BuildTextureShaderResource(ID3D12DescriptorHeap* InHeap, int Offset = 0);
private:
	unordered_map<wstring, unique_ptr<FRenderingTexture>> TextureMapping;
};

#endif
