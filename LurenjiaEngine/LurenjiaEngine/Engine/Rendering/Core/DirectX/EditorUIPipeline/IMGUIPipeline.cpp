#include "IMGUIPipeline.h"

#if EDITOR_ENGINE == 1
#include "../Editor/EditorEngine.h"
#endif // EDITOR_ENGINE == 1


FIMGUIPipeline::FIMGUIPipeline()
{
}

void FIMGUIPipeline::Init(ID3D12DescriptorHeap* InHeap, UINT InOffset)
{
	{// imgui初始化的内容，使用必须要有的

		IMGUI_CHECKVERSION();

		// ui上下文
		ImGui::CreateContext();

		// 颜色
		ImGui::StyleColorsDark();

		// 初始化win32 传入windows handle
		ImGui_ImplWin32_Init(GetMainWindowsHandle());

		UINT size = GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		auto cpu_handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(InHeap->GetCPUDescriptorHandleForHeapStart(), InOffset, size);

		auto gpu_handle = CD3DX12_GPU_DESCRIPTOR_HANDLE(
			InHeap->GetGPUDescriptorHandleForHeapStart());
		gpu_handle.Offset(InOffset, size);

		// 初始化dx12，记得要传进去堆，cpuhandle, gpuhandle
		ImGui_ImplDX12_Init(GetD3dDevice().Get(), 1, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, InHeap, cpu_handle, gpu_handle);
	}

	{// 下面是我自己写的逻辑
#if EDITOR_ENGINE == 1
		GetEditorEngine()->BuildEditor();


#endif // EDITOR_ENGINE == 1


	}
}

void FIMGUIPipeline::Draw(float DeltaTime)
{


	{// imgui初始化的内容，使用必须要有的

		// 创建了根签名，pso，字体
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();

		ImGui::NewFrame();

		// 具体绘制，测试用
		Tick(DeltaTime);

		ImGui::Render();

		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), GetGraphicsCommandList().Get());
	}

}

void FIMGUIPipeline::Tick(float DeltaTime)
{
	{// 全是测试用例
		//bool show_demo_window = true;

		//ImGui::ShowDemoWindow(&show_demo_window); // 官方提供的学习案例

		//ImGui::Begin("Log System");

		//ImGui::Text("adslfasd");
		//ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_Always); // 设置窗口位置
		//ImGui::SetWindowSize(ImGui::GetIO().DisplaySize); // 设置窗口大小，整个屏幕的大小
		//ImGui::Checkbox("bHeight", &bClicked);// 勾选框，能够改变bool的变量
		//float curNumber = 0.f;
		//ImGui::SliderFloat("Slider", &curNumber, 0.f, 1.f); // 滑动条
		//ImVec4 color = {1.f,1.f,1.f,1.f};
		//ImGui::ColorEdit3("Color", (float*)&color); // 改变颜色，获取颜色条

		//ImGui::End();
	}

#if EDITOR_ENGINE == 1
	GetEditorEngine()->DrawEditor(DeltaTime);
#endif
}

void FIMGUIPipeline::Exit()
{

#if EDITOR_ENGINE == 1
	GetEditorEngine()->ExitEditor();
#endif

	{// imgui初始化的内容，使用必须要有的，其中有一小段是自己的

		ImGui_ImplDX12_Shutdown();
		ImGui_ImplWin32_Shutdown();
	}
}
