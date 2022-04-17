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

	//向命令列表中 添加cbv描述符堆 命令
	ID3D12DescriptorHeap* DescriptorHeap[] = { CBVHeap.Get() };
	GraphicsCommandList->SetDescriptorHeaps(_countof(DescriptorHeap), DescriptorHeap);
	//向命令列表中 添加根签名 命令
	GraphicsCommandList->SetGraphicsRootSignature(RootSignature.Get());
	//向命令列表中 添加顶点缓冲数据 命令
	D3D12_VERTEX_BUFFER_VIEW VertexBufferView = GetVertexBufferView();
	GraphicsCommandList->IASetVertexBuffers(0, 1, &VertexBufferView);
	//向命令列表中 添加索引缓冲数据 命令
	D3D12_INDEX_BUFFER_VIEW IndexBufferView = GetIndexBufferView();
	GraphicsCommandList->IASetIndexBuffer(&IndexBufferView);
	//向命令列表中 添加图元拓扑 命令
	GraphicsCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//向命令列表中 添加将描述符表添加到根签名中 命令
	GraphicsCommandList->SetGraphicsRootDescriptorTable(0, CBVHeap->GetGPUDescriptorHandleForHeapStart());

	/// <summary>
	/// [in] UINT IndexCountPerInstance,每个绘制实例中索引的数量
	///	[in] UINT InstanceCount,		需要绘制的实例数量
	///	[in] UINT StartIndexLocation,	GPU从IndexBuffer读取的第一个index位置
	///	[in] INT  BaseVertexLocation,	给每个索引添加偏移，在读取VertexBuffer之前
	///	[in] UINT StartInstanceLocation	给每个索引添加偏移，在从VertexBuffer读取实例之前
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
	XMStoreFloat4x4(&ViewMatrix, ViewLookAt);						//创建摄像机矩阵,,,行优先

	XMMATRIX MatrixWorld = XMLoadFloat4x4(&WorldMatrix);
	XMMATRIX MatrixProject = XMLoadFloat4x4(&ProjectMatrix);
	XMMATRIX mvp = MatrixWorld * ViewLookAt * MatrixProject;		//行优先所以是右乘，，，列优先是左乘

	FObjectTransformation ObjectTransformation;
	XMStoreFloat4x4(&ObjectTransformation.World, XMMatrixTranspose(mvp));//装置是从行优先变成列优先

	objectConstants->Update(0, &ObjectTransformation);
}

void FMesh::BuildMesh(const FMeshRenderingData& InRenderingData)
{
//----------常量缓冲区的创建开始-----
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
//----------常量缓冲区的创建结束-----
//----------根签名的创建开始-----
	//创建根签名
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
//----------根签名的创建结束-----
//----------shader的创建开始-----
	VertexShader.BuildShader(L"../LurenjiaEngine/Shader/Hello.hlsl", "VertexShaderMain", "vs_5_0");
	PixelShader.BuildShader(L"../LurenjiaEngine/Shader/Hello.hlsl", "PixelShaderMain", "ps_5_0");
	InputElementDesc = {
		{"POSITION",0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};
//----------shader的创建结束-----
	IndexSize = InRenderingData.IndexData.size();
	VertexStrideInBytes = sizeof(FVertex);
	//获取顶点数据和索引的大小
	VertexSizeInBytes = InRenderingData.VertexData.size() * VertexStrideInBytes;
	IndexSizeInBytes = IndexSize * sizeof(uint16_t);

	//为将顶点索引数据 传递给cpuBuffer
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
//----------pos 流水线绑定开始-----
	D3D12_GRAPHICS_PIPELINE_STATE_DESC GPSDesc = {};
	memset(&GPSDesc, 0, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	//绑定输入布局
	GPSDesc.InputLayout.pInputElementDescs = InputElementDesc.data();
	GPSDesc.InputLayout.NumElements = (UINT)InputElementDesc.size();
	//绑定根签名
	GPSDesc.pRootSignature = RootSignature.Get();
	//绑定顶点着色器
	GPSDesc.VS.pShaderBytecode = reinterpret_cast<BYTE*>(VertexShader.GetBufferPointer());
	GPSDesc.VS.BytecodeLength = VertexShader.GetBufferSize();
	//绑定片元着色器
	GPSDesc.PS.pShaderBytecode = reinterpret_cast<BYTE*>(PixelShader.GetBufferPointer());
	GPSDesc.PS.BytecodeLength = PixelShader.GetBufferSize();
	//配置光栅化状态
	GPSDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//采样掩码
	GPSDesc.SampleMask = UINT_MAX;
	//拓扑类型
	GPSDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//渲染目标数量
	GPSDesc.NumRenderTargets = 1;
	//混合状态
	GPSDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	//深度模板
	GPSDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	//采样数量
	GPSDesc.SampleDesc.Count = GetEngine()->GetDXGISampleCount();
	//采样质量
	GPSDesc.SampleDesc.Quality = GetEngine()->GetDXGISampleQuality();
	//rtv格式
	GPSDesc.RTVFormats[0] = GetEngine()->GetBackBufferFormat();
	//dsv格式
	GPSDesc.DSVFormat = GetEngine()->GetDepthStencilFormat();
	//创建管线状态
	ANALYSIS_HRESULT(D3dDevice->CreateGraphicsPipelineState(&GPSDesc, IID_PPV_ARGS(&PSO)));
	
//----------pos 流水线绑定结束-----
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
