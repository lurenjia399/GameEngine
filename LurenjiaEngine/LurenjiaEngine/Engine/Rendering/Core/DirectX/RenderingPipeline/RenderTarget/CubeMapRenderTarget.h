#pragma once

#ifndef CUBEMAPRENDERTARGET_H
#define CUBEMAPRENDERTARGET_H

#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../RenderingResourcesUpdate.h"
#include "../../../../../Shader/Core/Shader.h"
#include "Core/RenderTarget.h"


// cubemap��rtv��Ҳ���ǻ�����ͨ��������Ⱦ
/*
* ������Ⱦ����ͨ��cpu׼��һЩ���ݣ�Ȼ�󴫵ݵ�gpu�н��м��㣬Ȼ�󽫽���ŵ�һ������Ҳ����rendertargetview����
* Ȼ����Ҫ��ʾ��ʱ���ͨ������������ʾ����
*/

class FCubeMapRenderTarget : public FRenderTarget
{
	typedef FRenderTarget Super;
public:
	// ��ʼ������
	FCubeMapRenderTarget();

	virtual void Init(UINT InWidth, UINT InHeight, DXGI_FORMAT InFormat) override;

	virtual void BuildRenderTargetResource() override;

	virtual void BuildShaderResourceDescriptorHandle() override;
	virtual void BuildShaderResourceView() override;

	void BuildRenderTargetDescriptorHandle();
	void BuildRenderTargetView();

	virtual void ResetRenderTarget(UINT InWidth, UINT InHeight) override;

public:
	// ���ⲿ�ṩ�Ľӿ�
	FORCEINLINE std::vector<CD3DX12_CPU_DESCRIPTOR_HANDLE>& GetRenderTargetDescriptor() { return RenderTargetDescriptor; }
	
private:
	std::vector<CD3DX12_CPU_DESCRIPTOR_HANDLE> RenderTargetDescriptor;
};


#endif // 
