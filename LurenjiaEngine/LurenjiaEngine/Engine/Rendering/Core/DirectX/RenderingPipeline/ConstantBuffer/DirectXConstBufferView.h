#pragma once

//���û�ж���Ԥ�������
#ifndef DirectXConstBufferView
#define DirectXConstBufferView

#include "../../../../../Core/Engine.h"
#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../RenderingResourcesUpdate.h"

struct FDirectXConstBufferView : public IDirectXDeviceInterface_struct
{
public:
	/// <summary>
	/// ������������ʾ�������Ƿ��ǳ�����������Ĭ��Ϊ��
	/// </summary>
	void CreateConstant(UINT BufferSize, UINT BufferCount, bool bIsConstantBuffer = true);
	void BuildConstantBuffer(CD3DX12_CPU_DESCRIPTOR_HANDLE InHandle, UINT ConstantBufferCount, UINT HandleOffset);
	void Update(int Index, const void* InData);
	ID3D12Resource* GetBuffer();
	UINT GetBufferByteSize();
private:
	shared_ptr<FRenderingResourcesUpdate> ConstantBufferView;	//����ĳ���������
};


#endif
