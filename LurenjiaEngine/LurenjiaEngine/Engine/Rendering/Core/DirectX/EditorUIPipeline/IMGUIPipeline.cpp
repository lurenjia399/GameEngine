#include "IMGUIPipeline.h"

FIMGUIPipeline::FIMGUIPipeline()
{
}

void FIMGUIPipeline::Init(ID3D12DescriptorHeap* InHeap, UINT InOffset)
{
	IMGUI_CHECKVERSION();

	// ui上下文
	ImGui::CreateContext();

	// 颜色
	ImGui::StyleColorsDark();

	// 初始化win32 传入windows handle
	ImGui_ImplWin32_Init(GetMainWindowsHandle());

	UINT size = GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	auto cpu_handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(InHeap->GetCPUDescriptorHandleForHeapStart(),InOffset,size);

	auto gpu_handle = CD3DX12_GPU_DESCRIPTOR_HANDLE(
		InHeap->GetGPUDescriptorHandleForHeapStart());
	gpu_handle.Offset(InOffset, size);

	ImGui_ImplDX12_Init(GetD3dDevice().Get(), 1, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, InHeap, cpu_handle, gpu_handle);
}

void FIMGUIPipeline::Draw(float DeltaTime)
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();



	ImGui::Render();

	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), GetGraphicsCommandList().Get());

}

void FIMGUIPipeline::Exit()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
}
