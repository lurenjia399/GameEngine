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
		// ������ǩ��
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
		// ����ˮƽshader
		HorzComputeShader.BuildShader(L"../LurenjiaEngine/Shader/Blur.hlsl", "HorzBlurCS", "cs_5_1");
		// ������ֱshader
		VertComputeShader.BuildShader(L"../LurenjiaEngine/Shader/Blur.hlsl", "VertBlurCS", "cs_5_1");
	}
	{
		// ���� ˮƽpso
		horzBlurPSO.pRootSignature = mPostProcessRootSignature.Get();
		horzBlurPSO.CS.pShaderBytecode = reinterpret_cast<BYTE*>(HorzComputeShader.GetBufferPointer());
		horzBlurPSO.CS.BytecodeLength = HorzComputeShader.GetBufferSize();
		horzBlurPSO.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
		ANALYSIS_HRESULT(GetD3dDevice()->CreateComputePipelineState(&horzBlurPSO, IID_PPV_ARGS(&pIHorzBlurPSO)));
		// ���� ��ֱpso
		VertBlurPSO.pRootSignature = mPostProcessRootSignature.Get();
		VertBlurPSO.CS.pShaderBytecode = reinterpret_cast<BYTE*>(VertComputeShader.GetBufferPointer());
		VertBlurPSO.CS.BytecodeLength = VertComputeShader.GetBufferSize();
		VertBlurPSO.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
		ANALYSIS_HRESULT(GetD3dDevice()->CreateComputePipelineState(&VertBlurPSO, IID_PPV_ARGS(&pIVertBlurPSO)));
	}

	{
		// ����srv��uav
		pIBlurMap0->BuildResource();
		pIBlurMap0->BuildSRVDesc();
		pIBlurMap0->BuildShaderResourceView(GetCBV_SRV_UAVHeap()->GetRenderingHeap(), 9);// ���Ӧ�����ڶ��е��������������Ҫ��
		pIBlurMap0->BuildUAVDesc();
		pIBlurMap0->BuildUnorderAccessView(GetCBV_SRV_UAVHeap()->GetRenderingHeap(), 10);// ���Ӧ�����ڶ��е��������������Ҫ��


		pIBlurMap1->BuildResource();
		pIBlurMap1->BuildSRVDesc();
		pIBlurMap1->BuildShaderResourceView(GetCBV_SRV_UAVHeap()->GetRenderingHeap(), 11);// ���Ӧ�����ڶ��е��������������Ҫ��
		pIBlurMap1->BuildUAVDesc();
		pIBlurMap1->BuildUnorderAccessView(GetCBV_SRV_UAVHeap()->GetRenderingHeap(), 12);// ���Ӧ�����ڶ��е��������������Ҫ��

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
	//��ʾģ���Ĵ����ı���
	UINT blurTimes = 10;
	{
		GetGraphicsCommandList()->SetComputeRootSignature(mPostProcessRootSignature.Get());
		//SetGraphicsRoot32BitConstants����:1.������������, ��Ӧ�Ĵ����Ĳۺ�2.�������ݸ���3.����ָ��4.ƫ����
		GetGraphicsCommandList()->SetComputeRoot32BitConstants(0, 1, &blurRadius, 0);
		GetGraphicsCommandList()->SetComputeRoot32BitConstants(0, (UINT)weights.size(), weights.data(), 1);
		
		// ��ǰ�������ϵ�rendertarget
		auto curRenderTargetResource = GetEngine()->GetRenderingEngine()->GetCurrentSwapBuffer();

		
		// rendertarget �� render_target -> copy_source
		// pIBlurMap0 �� state_common -> copy_dest
		D3D12_RESOURCE_BARRIER ResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
			curRenderTargetResource, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_COPY_SOURCE);
		GetGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrier);
		D3D12_RESOURCE_BARRIER ResourceBarrier2 = CD3DX12_RESOURCE_BARRIER::Transition(
			pIBlurMap0->GetBuffer(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
		GetGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrier2);

		GetGraphicsCommandList()->CopyResource(pIBlurMap0->GetBuffer(), curRenderTargetResource);

		// pIBlurMap0 �� copy_dest -> generic_read
		// pIBlurMap1 �� state_common -> unordered_access
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
			handle.Offset(9, size);// ���Ӧ�����ڶ��е��������������Ҫ��
			CD3DX12_GPU_DESCRIPTOR_HANDLE mBlur0GpuSrv = handle;
			handle.Offset(1, size);// ���Ӧ�����ڶ��е��������������Ҫ��
			CD3DX12_GPU_DESCRIPTOR_HANDLE mBlur0GpuUav = handle;
			handle.Offset(1, size);// ���Ӧ�����ڶ��е��������������Ҫ��
			CD3DX12_GPU_DESCRIPTOR_HANDLE mBlur1GpuSrv = handle;
			handle.Offset(1, size);// ���Ӧ�����ڶ��е��������������Ҫ��
			CD3DX12_GPU_DESCRIPTOR_HANDLE mBlur1GpuUav = handle;

			// ˮƽģ��
			GetGraphicsCommandList()->SetPipelineState(pIHorzBlurPSO.Get());
			GetGraphicsCommandList()->SetComputeRootDescriptorTable(1, mBlur0GpuSrv);
			GetGraphicsCommandList()->SetComputeRootDescriptorTable(2, mBlur1GpuUav);

			//�ϱ�һ��׼�������ˣ�����ֱ��Dispatch���ɣ�����֮����Ҫ����Դ״̬���е������������ִ�С�
			UINT numGroupsX = (UINT)ceilf(pIBlurMap0->iWidth / 256.0f);
			GetGraphicsCommandList()->Dispatch(numGroupsX, pIBlurMap0->iHeight, 1);// (4, 768, 1) �ֳɶ��ٸ��߳���

			// pIBlurMap0 �� generic_read -> unordered_access
			// pIBlurMap1 �� unordered_access -> generic_read
			D3D12_RESOURCE_BARRIER ResourceBarrier5 = CD3DX12_RESOURCE_BARRIER::Transition(
				pIBlurMap0->GetBuffer(), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
			GetGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrier5);
			D3D12_RESOURCE_BARRIER ResourceBarrier6 = CD3DX12_RESOURCE_BARRIER::Transition(
				pIBlurMap1->GetBuffer(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_GENERIC_READ);
			GetGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrier6);



			// ��ֱģ��
			GetGraphicsCommandList()->SetPipelineState(pIVertBlurPSO.Get());
			GetGraphicsCommandList()->SetComputeRootDescriptorTable(1, mBlur1GpuSrv);
			GetGraphicsCommandList()->SetComputeRootDescriptorTable(2, mBlur0GpuUav);

			UINT numGroupsY = (UINT)ceilf(pIBlurMap1->iHeight / 256.0f);
			GetGraphicsCommandList()->Dispatch(pIBlurMap1->iWidth, numGroupsY, 1);

			// rendertarget �� copy_source -> copy_dest
			// pIBlurMap0 �� unordered_access -> copy_source
			D3D12_RESOURCE_BARRIER ResourceBarrier7 = CD3DX12_RESOURCE_BARRIER::Transition(
				curRenderTargetResource, D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_COPY_DEST);
			GetGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrier7);
			D3D12_RESOURCE_BARRIER ResourceBarrier8 = CD3DX12_RESOURCE_BARRIER::Transition(
				pIBlurMap0->GetBuffer(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_SOURCE);
			GetGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrier8);
			
			GetGraphicsCommandList()->CopyResource(curRenderTargetResource, pIBlurMap0->GetBuffer());


			
			// pIBlurMap0 �� unordered_access -> generic_read
			// pIBlurMap1 �� generic_read -> unordered_access
			// rendertarget �� copy_dest -> copy_source
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

		// pIBlurMap0 �� generic_read -> state_common
		// pIBlurMap1 �� unordered_access -> state_common
		// rendertarget �� copy_source -> render_target
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
