#include "Mesh.h"
#include "../../Config/EngineRenderConfig.h"

FMesh::FMesh():
	VertexSizeInBytes(0),
	VertexStrideInBytes(0),
	IndexSizeInBytes(0),
	IndexFormat(DXGI_FORMAT_R16_UINT),
	IndexSize(0),
	WorldMatrix(FObjectTransformation::IdentityMatrix4x4()),
	ViewMatrix(FObjectTransformation::IdentityMatrix4x4()),
	ProjectMatrix(FObjectTransformation::IdentityMatrix4x4())
{
}

void FMesh::Init()
{
	float AspectRatio = static_cast<float>(FEngineRenderConfig::GetRenderConfig()->ScreenWidth) / FEngineRenderConfig::GetRenderConfig()->ScreenHeight;
	XMMATRIX Project = XMMatrixPerspectiveFovLH(0.25f * 3.1415926535f, AspectRatio, 1.0f, 1000.f);
	XMStoreFloat4x4(&ProjectMatrix, Project);
}

void FMesh::PreDraw(float DeltaTime)
{
	GetGraphicsCommandList()->Reset(GetCommandAllocator().Get(), PSO.Get());
}

void FMesh::Draw(float DeltaTime)
{
	ComPtr<ID3D12GraphicsCommandList> GraphicsCommandList = GetGraphicsCommandList();
	if (GraphicsCommandList == nullptr)
	{
		Engine_Log_Error("FMesh::Draw GraphicsCommandList is nullptr");
		return;
	}

	//�������б��� ���cbv�������� ����
	ID3D12DescriptorHeap* DescriptorHeap[] = { CBVHeap.Get() };
	GraphicsCommandList->SetDescriptorHeaps(_countof(DescriptorHeap), DescriptorHeap);
	//�������б��� ��Ӹ�ǩ�� ����
	GraphicsCommandList->SetGraphicsRootSignature(RootSignature.Get());
	//�������б��� ��Ӷ��㻺������ ����
	D3D12_VERTEX_BUFFER_VIEW VertexBufferView = GetVertexBufferView();
	GraphicsCommandList->IASetVertexBuffers(0, 1, &VertexBufferView);
	//�������б��� ��������������� ����
	D3D12_INDEX_BUFFER_VIEW IndexBufferView = GetIndexBufferView();
	GraphicsCommandList->IASetIndexBuffer(&IndexBufferView);
	//�������б��� ���ͼԪ���� ����
	GraphicsCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//�������б��� ��ӽ�����������ӵ���ǩ���� ����
	GraphicsCommandList->SetGraphicsRootDescriptorTable(0, CBVHeap->GetGPUDescriptorHandleForHeapStart());

	/// <summary>
	/// [in] UINT IndexCountPerInstance,ÿ������ʵ��������������
	///	[in] UINT InstanceCount,		��Ҫ���Ƶ�ʵ������
	///	[in] UINT StartIndexLocation,	GPU��IndexBuffer��ȡ�ĵ�һ��indexλ��
	///	[in] INT  BaseVertexLocation,	��ÿ���������ƫ�ƣ��ڶ�ȡVertexBuffer֮ǰ
	///	[in] UINT StartInstanceLocation	��ÿ���������ƫ�ƣ��ڴ�VertexBuffer��ȡʵ��֮ǰ
	/// </summary>
	/// <param name="DeltaTime"></param>
	
	// Draw Call !!!
	GraphicsCommandList->DrawIndexedInstanced(IndexSize, 1, 0, 0, 0);
}

void FMesh::PostDraw(float DeltaTime)
{
	XMUINT3 MeshPos = XMUINT3(5.0f, 5.0f, 5.0f);
	XMVECTOR Pos = XMVectorSet(MeshPos.x, MeshPos.y, MeshPos.z, 1.0f);
	XMVECTOR ViewTarget = XMVectorZero();
	XMVECTOR ViewUp = XMVectorSet(0, 1.0f, 0, 0);
	XMMATRIX ViewLookAt = XMMatrixLookAtLH(Pos, ViewTarget, ViewUp);
	XMStoreFloat4x4(&ViewMatrix, ViewLookAt);						//�������������,,,������

	XMMATRIX MatrixWorld = XMLoadFloat4x4(&WorldMatrix);
	XMMATRIX MatrixProject = XMLoadFloat4x4(&ProjectMatrix);
	XMMATRIX mvp = MatrixWorld * ViewLookAt * MatrixProject;		//�������������ҳˣ����������������

	FObjectTransformation ObjectTransformation;
	XMStoreFloat4x4(&ObjectTransformation.World, XMMatrixTranspose(mvp));//װ���Ǵ������ȱ��������

	objectConstants->Update(0, &ObjectTransformation);
}

void FMesh::BuildMesh(const FMeshRenderingData& InRenderingData)
{
//----------�����������Ĵ�����ʼ-----
	ComPtr<ID3D12Device> D3dDevice = GetD3dDevice();
	if (D3dDevice == nullptr)
	{
		Engine_Log_Error("FMesh::BuildMesh D3dDevice is nullptr");
		return;
	}
	objectConstants = make_shared<FRenderingResourcesUpdate>();
	objectConstants->Init(D3dDevice.Get(), sizeof(FObjectTransformation) , 1);
	D3D12_GPU_VIRTUAL_ADDRESS objadd = objectConstants->GetBuffer()->GetGPUVirtualAddress();
	D3D12_CONSTANT_BUFFER_VIEW_DESC CBVDesc = {};
	CBVDesc.BufferLocation = objadd;
	CBVDesc.SizeInBytes = objectConstants->GetConstantBufferByteSize();
	D3D12_DESCRIPTOR_HEAP_DESC CBVDescriptorHeapDesc = {};
	CBVDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	CBVDescriptorHeapDesc.NumDescriptors = 1;
	CBVDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	CBVDescriptorHeapDesc.NodeMask = 0;
	ANALYSIS_HRESULT(D3dDevice->CreateDescriptorHeap(&CBVDescriptorHeapDesc, IID_PPV_ARGS(&CBVHeap)));
	D3dDevice->CreateConstantBufferView(&CBVDesc, CBVHeap->GetCPUDescriptorHandleForHeapStart());
//----------�����������Ĵ�������-----
//----------��ǩ���Ĵ�����ʼ-----
	//������ǩ��
	CD3DX12_ROOT_PARAMETER RootParam[1];
	CD3DX12_DESCRIPTOR_RANGE DescriptorRangeCBV;
	DescriptorRangeCBV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	RootParam[0].InitAsDescriptorTable(1, &DescriptorRangeCBV);

	CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc(
		_countof(RootParam), RootParam, 0, nullptr,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
	);
	ComPtr<ID3DBlob> SerializeRootSignature;
	ComPtr<ID3DBlob> errorSerializeRootSignature;
	D3D12SerializeRootSignature(
		&RootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1,
		SerializeRootSignature.GetAddressOf(),
		errorSerializeRootSignature.GetAddressOf());
	if (errorSerializeRootSignature != nullptr)
	{
		Engine_Log_Error("root signature error = %s", (char*)errorSerializeRootSignature->GetBufferPointer());
		return;
	}
	ANALYSIS_HRESULT(D3dDevice->CreateRootSignature(
		0,
		SerializeRootSignature->GetBufferPointer(),
		SerializeRootSignature->GetBufferSize(),
		IID_PPV_ARGS(RootSignature.GetAddressOf())));
//----------��ǩ���Ĵ�������-----
//----------shader�Ĵ�����ʼ-----
	VertexShader.BuildShader(L"../LurenjiaEngine/Shader/Hello.hlsl", "VertexShaderMain", "vs_5_0");
	PixelShader.BuildShader(L"../LurenjiaEngine/Shader/Hello.hlsl", "PixelShaderMain", "ps_5_0");
	InputElementDesc = {
		{"POSITION",0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};
//----------shader�Ĵ�������-----
	IndexSize = InRenderingData.IndexData.size();
	VertexStrideInBytes = sizeof(FVertex);
	//��ȡ�������ݺ������Ĵ�С
	VertexSizeInBytes = InRenderingData.VertexData.size() * VertexStrideInBytes;
	IndexSizeInBytes = IndexSize * sizeof(uint16_t);

	//Ϊ�������������� ���ݸ�cpuBuffer
	ANALYSIS_HRESULT(D3DCreateBlob(VertexSizeInBytes, &CPUVertexBufferPtr));
	memcpy(CPUVertexBufferPtr->GetBufferPointer(), InRenderingData.VertexData.data(), VertexSizeInBytes);
	ANALYSIS_HRESULT(D3DCreateBlob(IndexSizeInBytes, &CPUIndexBufferPtr));
	memcpy(CPUIndexBufferPtr->GetBufferPointer(), InRenderingData.IndexData.data(), IndexSizeInBytes);

	GPUVertexBufferPtr = ConstructDefaultBuffer(VertexBufferTempPtr, InRenderingData.VertexData.data(), VertexSizeInBytes);
	GPUIndexBufferPtr = ConstructDefaultBuffer(IndexBufferTempPtr, InRenderingData.IndexData.data(), IndexSizeInBytes);
	if (GPUIndexBufferPtr == nullptr || GPUVertexBufferPtr == nullptr)
	{
		Engine_Log_Error("FMesh::BuildMesh discover error");
		return;
	}
//----------pos ��ˮ�߰󶨿�ʼ-----
	D3D12_GRAPHICS_PIPELINE_STATE_DESC GPSDesc = {};
	memset(&GPSDesc, 0, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	//�����벼��
	GPSDesc.InputLayout.pInputElementDescs = InputElementDesc.data();
	GPSDesc.InputLayout.NumElements = (UINT)InputElementDesc.size();
	//�󶨸�ǩ��
	GPSDesc.pRootSignature = RootSignature.Get();
	//�󶨶�����ɫ��
	GPSDesc.VS.pShaderBytecode = reinterpret_cast<BYTE*>(VertexShader.GetBufferPointer());
	GPSDesc.VS.BytecodeLength = VertexShader.GetBufferSize();
	//��ƬԪ��ɫ��
	GPSDesc.PS.pShaderBytecode = reinterpret_cast<BYTE*>(PixelShader.GetBufferPointer());
	GPSDesc.PS.BytecodeLength = PixelShader.GetBufferSize();
	//���ù�դ��״̬
	GPSDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//��������
	GPSDesc.SampleMask = UINT_MAX;
	//��������
	GPSDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//��ȾĿ������
	GPSDesc.NumRenderTargets = 1;
	//���״̬
	GPSDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	//���ģ��
	GPSDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	//��������
	GPSDesc.SampleDesc.Count = GetEngine()->GetDXGISampleCount();
	//��������
	GPSDesc.SampleDesc.Quality = GetEngine()->GetDXGISampleQuality();
	//rtv��ʽ
	GPSDesc.RTVFormats[0] = GetEngine()->GetBackBufferFormat();
	//dsv��ʽ
	GPSDesc.DSVFormat = GetEngine()->GetDepthStencilFormat();
	//��������״̬
	ANALYSIS_HRESULT(D3dDevice->CreateGraphicsPipelineState(&GPSDesc, IID_PPV_ARGS(&PSO)));
	
//----------pos ��ˮ�߰󶨽���-----
}

D3D12_VERTEX_BUFFER_VIEW FMesh::GetVertexBufferView()
{
	D3D12_VERTEX_BUFFER_VIEW VBV = {};
	VBV.BufferLocation = GPUVertexBufferPtr->GetGPUVirtualAddress();
	VBV.SizeInBytes = VertexSizeInBytes;
	VBV.StrideInBytes = VertexStrideInBytes;

	return VBV;
}

D3D12_INDEX_BUFFER_VIEW FMesh::GetIndexBufferView()
{
	D3D12_INDEX_BUFFER_VIEW IBV = {};
	IBV.SizeInBytes = IndexSizeInBytes;
	IBV.Format = IndexFormat;
	IBV.BufferLocation = GPUIndexBufferPtr->GetGPUVirtualAddress();

	return IBV;
}

FObjectTransformation::FObjectTransformation()
	: World(IdentityMatrix4x4())
{
}

XMFLOAT4X4 FObjectTransformation::IdentityMatrix4x4()
{
	return XMFLOAT4X4(
			1.0f, 0, 0, 0,
			0, 1.0f, 0, 0,
			0, 0, 1.0f, 0,
			0, 0, 0, 1.0f);
}
