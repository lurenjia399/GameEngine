#include "ComputePipeline.h"
#include "../../../Engine/DirectX/Core/DirectXRenderingEngine.h"

FComputePipeline::FComputePipeline()
	: horzBlurPSO({})
	, pIHorzBlurPSO(ComPtr<ID3D12PipelineState>())
	, HorzComputeShader({})
	, VertBlurPSO({})
	, pIVertBlurPSO(ComPtr<ID3D12PipelineState>())
	, VertComputeShader({})
	, mPostProcessRootSignature(ComPtr<ID3D12RootSignature>())
	, pIBlurMap0(make_shared<FRenderingUAVResourvesUpdate>())
	, pIBlurMap1(make_shared<FRenderingUAVResourvesUpdate>())
{
}

void FComputePipeline::BuildPipeline()
{
	{
		// 创建根签名
		CD3DX12_ROOT_PARAMETER RootParam[3];

		CD3DX12_DESCRIPTOR_RANGE srvTable = {};
		srvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

		CD3DX12_DESCRIPTOR_RANGE uavTable = {};
		uavTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0);

		RootParam[0].InitAsConstants(12, 0);
		RootParam[1].InitAsDescriptorTable(1, &srvTable);
		RootParam[2].InitAsDescriptorTable(1, &uavTable);

		CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc(
			3, RootParam, 0, nullptr,
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		ComPtr<ID3DBlob> SerializeRootSignature = nullptr;
		ComPtr<ID3DBlob> errorSerializeRootSignature = nullptr;
		D3D12SerializeRootSignature(
			&RootSignatureDesc,
			D3D_ROOT_SIGNATURE_VERSION_1,
			SerializeRootSignature.GetAddressOf(),
			errorSerializeRootSignature.GetAddressOf());

		if (errorSerializeRootSignature != nullptr)
		{
			Engine_Log_Error("root signature error = %s", (char*)errorSerializeRootSignature->GetBufferPointer());
		}

		ANALYSIS_HRESULT(GetD3dDevice()->CreateRootSignature(
			0,
			SerializeRootSignature->GetBufferPointer(),
			SerializeRootSignature->GetBufferSize(),
			IID_PPV_ARGS(&mPostProcessRootSignature)));
	}

	{
		// 创建水平shader
		HorzComputeShader.BuildShader(L"../LurenjiaEngine/Shader/Blur.hlsl", "HorzBlurCS", "cs_5_1");
		// 创建竖直shader
		VertComputeShader.BuildShader(L"../LurenjiaEngine/Shader/Blur.hlsl", "VertBlurCS", "cs_5_1");
	}
	{
		// 创建 水平pso
		horzBlurPSO.pRootSignature = mPostProcessRootSignature.Get();
		horzBlurPSO.CS.pShaderBytecode = reinterpret_cast<BYTE*>(HorzComputeShader.GetBufferPointer());
		horzBlurPSO.CS.BytecodeLength = HorzComputeShader.GetBufferSize();
		horzBlurPSO.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
		ANALYSIS_HRESULT(GetD3dDevice()->CreateComputePipelineState(&horzBlurPSO, IID_PPV_ARGS(&pIHorzBlurPSO)));
		// 创建 竖直pso
		VertBlurPSO.pRootSignature = mPostProcessRootSignature.Get();
		VertBlurPSO.CS.pShaderBytecode = reinterpret_cast<BYTE*>(VertComputeShader.GetBufferPointer());
		VertBlurPSO.CS.BytecodeLength = VertComputeShader.GetBufferSize();
		VertBlurPSO.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
		ANALYSIS_HRESULT(GetD3dDevice()->CreateComputePipelineState(&VertBlurPSO, IID_PPV_ARGS(&pIVertBlurPSO)));
	}

	{
		// 创建srv，uav
		pIBlurMap0->BuildResource();
		pIBlurMap0->BuildSRVDesc();
		pIBlurMap0->BuildShaderResourceView(GetCBV_SRV_UAVHeap()->GetRenderingHeap(), 9);// 序号应该填在堆中的索引，这里必须要改
		pIBlurMap0->BuildUAVDesc();
		pIBlurMap0->BuildUnorderAccessView(GetCBV_SRV_UAVHeap()->GetRenderingHeap(), 10);// 序号应该填在堆中的索引，这里必须要改


		pIBlurMap1->BuildResource();
		pIBlurMap1->BuildSRVDesc();
		pIBlurMap1->BuildShaderResourceView(GetCBV_SRV_UAVHeap()->GetRenderingHeap(), 11);// 序号应该填在堆中的索引，这里必须要改
		pIBlurMap1->BuildUAVDesc();
		pIBlurMap1->BuildUnorderAccessView(GetCBV_SRV_UAVHeap()->GetRenderingHeap(), 12);// 序号应该填在堆中的索引，这里必须要改

	}
}

void FComputePipeline::Draw()
{
	std::function<std::vector<float>(float)> CalcGaussWeights  = [](float sigma) -> std::vector<float>
	{
		float twoSigma2 = 2.0f * sigma * sigma;

		// Estimate the blur radius based on sigma since sigma controls the "width" of the bell curve.
		// For example, for sigma = 3, the width of the bell curve is 
		int blurRadius = (int)ceil(2.0f * sigma);


		std::vector<float> weights;
		weights.resize(2 * blurRadius + 1);

		float weightSum = 0.0f;

		for (int i = -blurRadius; i <= blurRadius; ++i)
		{
			float x = (float)i;

			weights[i + blurRadius] = expf(-x * x / twoSigma2);

			weightSum += weights[i + blurRadius];
		}

		// Divide by the sum so all the weights add up to 1.0.
		for (UINT i = 0; i < weights.size(); ++i)
		{
			weights[i] /= weightSum;
		}

		return weights;
	};

	auto weights = CalcGaussWeights(2.5f);
	int blurRadius = (int)weights.size() / 2;
	//表示模糊的次数的变量
	UINT blurTimes = 10;
	{
		GetGraphicsCommandList()->SetComputeRootSignature(mPostProcessRootSignature.Get());
		//SetGraphicsRoot32BitConstants参数:1.根参数的索引, 对应寄存器的槽号2.常量数据个数3.数据指针4.偏移量
		GetGraphicsCommandList()->SetComputeRoot32BitConstants(0, 1, &blurRadius, 0);
		GetGraphicsCommandList()->SetComputeRoot32BitConstants(0, (UINT)weights.size(), weights.data(), 1);
		
		// 当前交换链上的rendertarget
		auto curRenderTargetResource = GetEngine()->GetRenderingEngine()->GetCurrentSwapBuffer();

		
		// rendertarget 从 render_target -> copy_source
		// pIBlurMap0 从 state_common -> copy_dest
		D3D12_RESOURCE_BARRIER ResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
			curRenderTargetResource, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_COPY_SOURCE);
		GetGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrier);
		D3D12_RESOURCE_BARRIER ResourceBarrier2 = CD3DX12_RESOURCE_BARRIER::Transition(
			pIBlurMap0->GetBuffer(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
		GetGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrier2);

		GetGraphicsCommandList()->CopyResource(pIBlurMap0->GetBuffer(), curRenderTargetResource);

		// pIBlurMap0 从 copy_dest -> generic_read
		// pIBlurMap1 从 state_common -> unordered_access
		D3D12_RESOURCE_BARRIER ResourceBarrier3 = CD3DX12_RESOURCE_BARRIER::Transition(
			pIBlurMap0->GetBuffer(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
		GetGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrier3);
		D3D12_RESOURCE_BARRIER ResourceBarrier4 = CD3DX12_RESOURCE_BARRIER::Transition(
			pIBlurMap1->GetBuffer(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
		GetGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrier4);

		UINT size = GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		for (UINT i = 0; i < blurTimes; i++)
		{
			
			CD3DX12_GPU_DESCRIPTOR_HANDLE handle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetCBV_SRV_UAVHeap()->GetRenderingHeap()->GetGPUDescriptorHandleForHeapStart());
			handle.Offset(9, size);// 序号应该填在堆中的索引，这里必须要改
			CD3DX12_GPU_DESCRIPTOR_HANDLE mBlur0GpuSrv = handle;
			handle.Offset(1, size);// 序号应该填在堆中的索引，这里必须要改
			CD3DX12_GPU_DESCRIPTOR_HANDLE mBlur0GpuUav = handle;
			handle.Offset(1, size);// 序号应该填在堆中的索引，这里必须要改
			CD3DX12_GPU_DESCRIPTOR_HANDLE mBlur1GpuSrv = handle;
			handle.Offset(1, size);// 序号应该填在堆中的索引，这里必须要改
			CD3DX12_GPU_DESCRIPTOR_HANDLE mBlur1GpuUav = handle;

			// 水平模糊
			GetGraphicsCommandList()->SetPipelineState(pIHorzBlurPSO.Get());
			GetGraphicsCommandList()->SetComputeRootDescriptorTable(1, mBlur0GpuSrv);
			GetGraphicsCommandList()->SetComputeRootDescriptorTable(2, mBlur1GpuUav);

			//上边一切准备就绪了，这里直接Dispatch即可，完了之后需要把资源状态进行调整便于下面的执行。
			UINT numGroupsX = (UINT)ceilf(pIBlurMap0->iWidth / 256.0f);
			GetGraphicsCommandList()->Dispatch(numGroupsX, pIBlurMap0->iHeight, 1);// (4, 768, 1) 分成多少个线程组

			// pIBlurMap0 从 generic_read -> unordered_access
			// pIBlurMap1 从 unordered_access -> generic_read
			D3D12_RESOURCE_BARRIER ResourceBarrier5 = CD3DX12_RESOURCE_BARRIER::Transition(
				pIBlurMap0->GetBuffer(), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
			GetGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrier5);
			D3D12_RESOURCE_BARRIER ResourceBarrier6 = CD3DX12_RESOURCE_BARRIER::Transition(
				pIBlurMap1->GetBuffer(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_GENERIC_READ);
			GetGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrier6);



			// 垂直模糊
			GetGraphicsCommandList()->SetPipelineState(pIVertBlurPSO.Get());
			GetGraphicsCommandList()->SetComputeRootDescriptorTable(1, mBlur1GpuSrv);
			GetGraphicsCommandList()->SetComputeRootDescriptorTable(2, mBlur0GpuUav);

			UINT numGroupsY = (UINT)ceilf(pIBlurMap1->iHeight / 256.0f);
			GetGraphicsCommandList()->Dispatch(pIBlurMap1->iWidth, numGroupsY, 1);

			// rendertarget 从 copy_source -> copy_dest
			// pIBlurMap0 从 unordered_access -> copy_source
			D3D12_RESOURCE_BARRIER ResourceBarrier7 = CD3DX12_RESOURCE_BARRIER::Transition(
				curRenderTargetResource, D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_COPY_DEST);
			GetGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrier7);
			D3D12_RESOURCE_BARRIER ResourceBarrier8 = CD3DX12_RESOURCE_BARRIER::Transition(
				pIBlurMap0->GetBuffer(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_SOURCE);
			GetGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrier8);
			
			GetGraphicsCommandList()->CopyResource(curRenderTargetResource, pIBlurMap0->GetBuffer());


			
			// pIBlurMap0 从 unordered_access -> generic_read
			// pIBlurMap1 从 generic_read -> unordered_access
			// rendertarget 从 copy_dest -> copy_source
			D3D12_RESOURCE_BARRIER ResourceBarrier9 = CD3DX12_RESOURCE_BARRIER::Transition(
				pIBlurMap0->GetBuffer(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_GENERIC_READ);
			GetGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrier9);
			D3D12_RESOURCE_BARRIER ResourceBarrier10 = CD3DX12_RESOURCE_BARRIER::Transition(
				pIBlurMap1->GetBuffer(), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
			GetGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrier10);
			D3D12_RESOURCE_BARRIER ResourceBarrier11 = CD3DX12_RESOURCE_BARRIER::Transition(
				curRenderTargetResource, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_COPY_SOURCE);
			GetGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrier11);


		}

		// pIBlurMap0 从 generic_read -> state_common
		// pIBlurMap1 从 unordered_access -> state_common
		// rendertarget 从 copy_source -> render_target
		D3D12_RESOURCE_BARRIER ResourceBarrier5 = CD3DX12_RESOURCE_BARRIER::Transition(
			pIBlurMap0->GetBuffer(), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_COMMON);
		GetGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrier5);
		D3D12_RESOURCE_BARRIER ResourceBarrier6 = CD3DX12_RESOURCE_BARRIER::Transition(
			pIBlurMap1->GetBuffer(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COMMON);
		GetGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrier6);
		D3D12_RESOURCE_BARRIER ResourceBarrier7 = CD3DX12_RESOURCE_BARRIER::Transition(
			curRenderTargetResource, D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
		GetGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrier7);

	}
	
}
