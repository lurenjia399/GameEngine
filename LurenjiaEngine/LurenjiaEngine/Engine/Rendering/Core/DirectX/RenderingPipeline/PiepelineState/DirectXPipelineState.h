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
	/// ����ɫ�����벼��
	/// </summary>
	/// <param name="InInputElementDescs">��������</param>
	/// <param name="InSize">�����С</param>
	void BindInputLayout(const D3D12_INPUT_ELEMENT_DESC* InInputElementDescs, UINT InSize);
	/// <summary>
	/// �󶨸�ǩ��
	/// </summary>
	/// <param name="RootSignature">��ǩ��</param>
	void BindRootSignature(ID3D12RootSignature* RootSignature);
	/// <summary>
	/// ����ɫ��
	/// </summary>
	/// <param name="VertexShader">������ɫ��</param>
	/// <param name="PixelShader">ƬԪ��ɫ��</param>
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
	ERenderingPiepelineState CurrPipelineType;				//��Ⱦģʽ���߿���������Σ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC GPSDesc;				//����״̬����
};