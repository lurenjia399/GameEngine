#include "IMGUIPipeline.h"

#if EDITOR_ENGINE == 1
#include "../Editor/EditorEngine.h"
#endif // EDITOR_ENGINE == 1


FIMGUIPipeline::FIMGUIPipeline()
{
}

void FIMGUIPipeline::Init(ID3D12DescriptorHeap* InHeap, UINT InOffset)
{
	{// imgui��ʼ�������ݣ�ʹ�ñ���Ҫ�е�

		IMGUI_CHECKVERSION();

		// ui������
		ImGui::CreateContext();

		// ��ɫ
		ImGui::StyleColorsDark();

		// ��ʼ��win32 ����windows handle
		ImGui_ImplWin32_Init(GetMainWindowsHandle());

		UINT size = GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		auto cpu_handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(InHeap->GetCPUDescriptorHandleForHeapStart(), InOffset, size);

		auto gpu_handle = CD3DX12_GPU_DESCRIPTOR_HANDLE(
			InHeap->GetGPUDescriptorHandleForHeapStart());
		gpu_handle.Offset(InOffset, size);

		// ��ʼ��dx12���ǵ�Ҫ����ȥ�ѣ�cpuhandle, gpuhandle
		ImGui_ImplDX12_Init(GetD3dDevice().Get(), 1, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, InHeap, cpu_handle, gpu_handle);
	}

	{// ���������Լ�д���߼�
#if EDITOR_ENGINE == 1
		GetEditorEngine()->BuildEditor();


#endif // EDITOR_ENGINE == 1


	}
}

void FIMGUIPipeline::Draw(float DeltaTime)
{


	{// imgui��ʼ�������ݣ�ʹ�ñ���Ҫ�е�

		// �����˸�ǩ����pso������
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();

		ImGui::NewFrame();

		// ������ƣ�������
		Tick(DeltaTime);

		ImGui::Render();

		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), GetGraphicsCommandList().Get());
	}

}

void FIMGUIPipeline::Tick(float DeltaTime)
{
	{// ȫ�ǲ�������
		//bool show_demo_window = true;
		//ImGui::ShowDemoWindow(&show_demo_window); // �ٷ��ṩ��ѧϰ����

		//ImGui::Begin("Log System");

		//ImGui::Text("adslfasd");
		//ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_Always); // ���ô���λ��
		//ImGui::SetWindowSize(ImGui::GetIO().DisplaySize); // ���ô��ڴ�С��������Ļ�Ĵ�С
		//ImGui::Checkbox("bHeight", &bClicked);// ��ѡ���ܹ��ı�bool�ı���
		//float curNumber = 0.f;
		//ImGui::SliderFloat("Slider", &curNumber, 0.f, 1.f); // ������
		//ImVec4 color = {1.f,1.f,1.f,1.f};
		//ImGui::ColorEdit3("Color", (float*)&color); // �ı���ɫ����ȡ��ɫ��

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

	{// imgui��ʼ�������ݣ�ʹ�ñ���Ҫ�еģ�������һС�����Լ���

		ImGui_ImplDX12_Shutdown();
		ImGui_ImplWin32_Shutdown();
	}
}