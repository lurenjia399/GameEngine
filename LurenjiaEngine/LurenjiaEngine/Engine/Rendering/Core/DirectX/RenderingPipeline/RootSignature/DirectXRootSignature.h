#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../RenderingResourcesUpdate.h"


struct FDirectXRootSignature : public IDirectXDeviceInterface_struct
{
public:
	FDirectXRootSignature();
	bool Build();
	ID3D12RootSignature* GetRootSignature();
	void PreDraw(float DeltaTime);
	void Draw(float DeltaTime);
	void PostDraw(float DeltaTime);
private:
	ComPtr<ID3D12RootSignature> RootSignature;
};