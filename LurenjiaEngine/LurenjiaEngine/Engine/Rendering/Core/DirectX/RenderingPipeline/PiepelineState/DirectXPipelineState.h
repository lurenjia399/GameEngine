#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../RenderingResourcesUpdate.h"
#include "../../../../../Shader/Core/Shader.h"
#include "../RenderingPiepelineType.h"


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
	/// Build PsoDesc
	/// </summary>
	void BuildParam(D3D12_FILL_MODE InFillMode, D3D12_CULL_MODE InCullMode);

	/// <summary>
	/// Build Pso
	/// </summary>
	void Build(int InPipelineType);

	void ResetPSO(int InPiepelType);

	void PreDraw(float DeltaTime);
	void Draw(float DeltaTime);
	void PostDraw(float DeltaTime);
public:
	void SetFillMode(D3D12_FILL_MODE InFillMode);
	void SetCullMode(D3D12_CULL_MODE InCullMode);

private:
	void CaptureKeyboardKeys();
private:
	unordered_map<int, ComPtr<ID3D12PipelineState>> PSO;	//pso
	ERenderingPiepelineState CurrPipelineType;				//渲染模式（线框或者三角形）
	D3D12_GRAPHICS_PIPELINE_STATE_DESC GPSDesc;				//管线状态描述
};