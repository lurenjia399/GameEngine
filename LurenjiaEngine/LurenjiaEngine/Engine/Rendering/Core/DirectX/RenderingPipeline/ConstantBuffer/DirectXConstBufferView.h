#pragma once

//如果没有定义预处理变量
#ifndef DirectXConstBufferView
#define DirectXConstBufferView

#include "../../../../../Core/Engine.h"
#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../RenderingResourcesUpdate.h"

struct FDirectXConstBufferView : public IDirectXDeviceInterface_struct
{
public:
	void CreateConstant(UINT BufferSize, UINT BufferCount, bool bIsConstantBuffer = true);
	void BuildConstantBuffer(CD3DX12_CPU_DESCRIPTOR_HANDLE InHandle, UINT ConstantBufferCount, UINT HandleOffset);
	void Update(int Index, const void* InData);
	ID3D12Resource* GetBuffer();
private:
	shared_ptr<FRenderingResourcesUpdate> ConstantBufferView;	//对象的常量缓冲区
};


#endif
