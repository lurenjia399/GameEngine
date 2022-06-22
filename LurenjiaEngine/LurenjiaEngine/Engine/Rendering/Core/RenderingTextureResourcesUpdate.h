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
	wstring AssertFilename;//��Դ·�� Texture"Assert/Texture/Hello"
	wstring SimpleAssertFilename;//�򵥵���Դ·�� "Assert/Texture/Hello"
	UINT RenderingTextureID;
};

class FRenderingTextureResourcesUpdate : public enable_shared_from_this<FRenderingTextureResourcesUpdate>, public IDirectXDeviceInterface
{
public:
	void LoadTexture(const wstring& InTexturePath);
	void BuildTextureShaderResource(ID3D12DescriptorHeap* InHeap, int Offset = 0);
	int GetTextureCount() const;
	int GetTextureIndex(string InKey);
private:
	std::map<wstring, unique_ptr<FRenderingTexture>> TextureMapping;
};

#endif
