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

	void Build();

	void PreDraw(float DeltaTime);
	void Draw(float DeltaTime);
	void PostDraw(float DeltaTime);

private:
	void CaptureKeyboardKeys();
private:
	unordered_map<int, ComPtr<ID3D12PipelineState>> PSO;						//����״̬
	ERenderingPiepelineState CurrPipelineType;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC GPSDesc;				//����״̬����
};