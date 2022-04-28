#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../RenderingResourcesUpdate.h"
#include "../../../../../Shader/Core/Shader.h"


struct FDirectXPiepelineState : public IDirectXDeviceInterface_struct
{
public:
	FDirectXPiepelineState();
	void ResetGPSDesc();
	/// <summary>
	/// 绑定着色器输入布局
	/// </summary>
	/// <param name="InInputElementDescs">输入描述</param>
	/// <param name="InSize">输入大小</param>
	void BindInputLayout(const D3D12_INPUT_ELEMENT_DESC* InInputElementDescs, UINT InSize);
	/// <summary>
	/// 绑定根签名
	/// </summary>
	/// <param name="RootSignature">根签名</param>
	void BindRootSignature(ID3D12RootSignature* RootSignature);
	/// <summary>
	/// 绑定着色器
	/// </summary>
	/// <param name="VertexShader">顶点着色器</param>
	/// <param name="PixelShader">片元着色器</param>
	void BindShader(const FShader* VertexShader, const FShader* PixelShader);
	/// <summary>
	/// 配置光栅化参数
	/// </summary>
	void BindRasterizerState();

	void Build();
private:
	ComPtr<ID3D12PipelineState> PSO;						//管线状态
	
	D3D12_GRAPHICS_PIPELINE_STATE_DESC GPSDesc;				//管线状态描述
};