#include "RenderingPipeline.h"

#define  A 0  //��A����Ϊ0
enum class EPiepelineStateType;

FRenderingPipeline::FRenderingPipeline()
{
}

bool FRenderingPipeline::FindMeshRenderingDataByHash(const size_t& InHashKey, std::weak_ptr<FGeometryDescData>& OutGeometryDescData, int InRenderingLayer)
{
	return GeometryMap.FindMeshRenderingDataByHash(InHashKey, OutGeometryDescData, InRenderingLayer);
}

void FRenderingPipeline::DuplicateMeshRenderingData(std::shared_ptr<CMeshComponent> InMesh, std::weak_ptr<FGeometryDescData>& InGeometryDescData)
{
	GeometryMap.DuplicateMeshRenderingData(InMesh, InGeometryDescData);
}

void FRenderingPipeline::BuildMeshComponent(std::shared_ptr<CMeshComponent> InMesh, const FMeshRenderingData& InRenderingData, const size_t& HashKey)
{
	GeometryMap.BuildMeshDescData(InMesh, InRenderingData, HashKey);
}

void FRenderingPipeline::BuildPipeline()
{
	DirectXPiepelineState.ResetGPSDesc();

	{
		//��ʼ����Ⱦ�㼶
		FRenderLayerManage::GetRenderLayerManage()->Init(&GeometryMap, &DirectXPiepelineState);
		//����Ⱦ�㼶��������
		FRenderLayerManage::GetRenderLayerManage()->sort();
	}

	//������ͼ��Դ
	GeometryMap.LoadTexture();

	//�󶨸�ǩ��
	if (!DirectXRootSignature.Build(GeometryMap.GetDrawTextureObjectCount()))
	{
		Engine_Log_Error("������ǩ��ʧ��");
	}
	DirectXPiepelineState.BindRootSignature(DirectXRootSignature.GetRootSignature());

	{
		/*
		* �ⲿ����Դ��ģ�͵Ķ��㡢��������ɶ��
		* ��Щ������cpu���㣬Ȼ��ͨ���ϴ��ѣ������ݷŵ�Ĭ�϶����棬��gpu��ȡ
		*/
		//����ģ����Դ�������������������
		GeometryMap.BuildMeshBuffer();
	}

	{
		//������̬����Mesh
		GeometryMap.BuildDynamicReflectionMesh();
	}

	{
		/*
		* �ⲿ����ԴӦ���ǳ�����Ҳ����ͨ��cpu���������ݣ�Ȼ��ͨ�����������ϴ���ӳ�䣬������ͬ�ڷŵ��ϴ�����
		* gpuÿ����Ⱦ��ʱ���ȡ�ϴ����е����ݣ���������ɫ���м���
		* ÿ����Ⱦtick�����ݶ�����cpu���¼�����£����Է����ϴ�����
		*/
		//���������ϴ��ѣ�Ҳ���Ǵ����ϴ��ѣ������������ݺ��ϴ���ӳ�������������⼸��ͬ����ˣ�
		GeometryMap.BuildFogConstantBufferView();
		//����ģ�͵��ϴ���
		GeometryMap.BuildMeshConstantBufferView();
		//�������ʵ��ϴ���,�ռ����е�matrial����������material�е�shaderindex��
		GeometryMap.BuildMaterialShaderResourseView();
		//�����ƹ���ϴ���
		GeometryMap.BuildLightConstantBufferView();
		//�����ӿڵ��ϴ���
		GeometryMap.BuildViewportConstantBufferView(0);
	}

	{
		/*
		* �ⲿ����Դ����ͼ��Դ��Ӧ�����ڳ������е��в���ı䣬Ȼ��Ͱ����Ǵ�������CreateShaderResourceView
		* Ȼ�����һ�������������
		*/
		//�����������ѣ����ڴ����ͼ��Դ��
		GeometryMap.BuildDescriptorHeap();
		//������ͼ����ɫ����Դ�������Ƿŵ��ϱߵ����������е��������У�Ҳ���ǵ�������Ԫ��
		GeometryMap.BuildTextureShaderResource();
	}

	{
		// ������̬��cubemap
		DynamicCubeMap.Init(&GeometryMap, &DirectXPiepelineState);
		DynamicCubeMap.BuildViewport(XMFLOAT3(0.f, 5.f, 2.f)); // �Ǹ��������λ��

		//�������ģ���������������ס�Ⱥ�˳���ȹ������Ȼ���ڹ�����������
		//DynamicCubeMap.BuildDepthStencilDescriptorHandle();
		//�������ģ��������
		//DynamicCubeMap.BuildDepthStencilView();

		//����RTV��SRV
		DynamicCubeMap.BuildRenderTargetDescriptor();
	}

	{
		// ��ʼ����Ӱ
		GeometryMap.InitDynamicShadowMap(&GeometryMap, &DirectXPiepelineState);
		GeometryMap.BuildShadowMap();
	}

	//����pso
	FRenderLayerManage::GetRenderLayerManage()->BuildPSO();

	// ������ɫ������
	{

#if (OPENCOMPUTEPIPELINE == 1)
		// ������ɫ��
		ComputePipeline.BuildPipeline();
		Engine_Log("cur open compute pipeline ? Yes")
#else
		Engine_Log_Warning("cur open compute pipeline ? No")
#endif
	}

	// Eidtor UI ����
	{
		EditorUIPipeline.Init(GeometryMap.GetCBV_SRV_UAVHeap()->GetRenderingHeap(), 
			GeometryMap.GetDrawTextureObjectCount() //texture2d 6��
			+ GeometryMap.GetDrawCubeMapCount() //��̬cubemap 1��
			+ 1	// ��̬cubemap
			+ 1 // ��Ӱ
#if (OPENCOMPUTEPIPELINE == 1)
			+ 2 // computeShader pIBlurMap0 ��SRV��UAV
			+ 2 // computeShader pIBlurMap1 ��SRV��UAV
#endif
		);
	}
	
}

void FRenderingPipeline::UpdateConstantView(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	//���¶�̬�����ӿڳ���������
	DynamicCubeMap.UpdateViewportConstantBufferView(DeltaTime, ViewportInfo);
	//����ÿ���㼶��ģ�͵ĳ�����������Դ
	FRenderLayerManage::GetRenderLayerManage()->UpdateObjectConstantBuffer();
	//���²��ʳ���������
	GeometryMap.UpdateMaterialShaderResourceView(DeltaTime);
	//���µƹ�ĳ���������
	GeometryMap.UpdateLightConstantBufferView(DeltaTime);
	//�����ӿڵĳ���������
	GeometryMap.UpdateViewportConstantBufferView(DeltaTime, ViewportInfo, 0);
	//���������ĳ���������
	GeometryMap.UpdateFogConstantBufferView(DeltaTime);
	//������Ӱ�ĳ���������
	GeometryMap.UpdateShadowMapShaderResourceView(DeltaTime, ViewportInfo);
}

void FRenderingPipeline::PreDraw(float DeltaTime)
{
	// �������б������� ����pso
	DirectXPiepelineState.PreDraw(DeltaTime);

	// �������б������� �������ѣ��������洢�ĸ��ֲ������Դ����ͼ
	GeometryMap.PreDraw(DeltaTime);
	// �������б������� ��ǩ��
	DirectXRootSignature.PreDraw(DeltaTime);

	// ���rtv����Ķ���
	ClearMainSwapChain();

	// ����������ǰ󶨸�ǩ������߻�����Ⱦ��Ӱ
	GeometryMap.Draw(DeltaTime);

	


	// ��Ⱦ��̬����
	if (DynamicCubeMap.IsExitDynamicReflectionMesh())
	{
		DynamicCubeMap.PreDraw(DeltaTime);
	}
	


	// �����������ɶ��û��
	FRenderLayerManage::GetRenderLayerManage()->PreDraw(DeltaTime);
	
}

void FRenderingPipeline::Draw(float DeltaTime)
{
	
	GeometryMap.DrawViewport(DeltaTime);

	// ������ȫ�Ǹ��ݸ�ǩ������ţ�����gpu�ڴ��ַ��Ҳ���Ǹ�����ɫ����Ӧ����Դ�����ĸ��Ĵ���������
	GeometryMap.DrawCubeMapTexture(DeltaTime);

	// Drawÿ����Ⱦ�㼶 ���� -> ��͸������ -> ͸������ -> AlphaTest -> Select
	FRenderLayerManage::GetRenderLayerManage()->Draw((int)EMeshComponentRenderLayerType::RENDERLAYER_BACKGROUND, DeltaTime);
	FRenderLayerManage::GetRenderLayerManage()->Draw((int)EMeshComponentRenderLayerType::RENDERLAYER_OPAQUE, DeltaTime);
	FRenderLayerManage::GetRenderLayerManage()->Draw((int)EMeshComponentRenderLayerType::RENDERLAYER_TRANSPARENT, DeltaTime);
	FRenderLayerManage::GetRenderLayerManage()->Draw((int)EMeshComponentRenderLayerType::RENDERLAYER_ALPHATEST, DeltaTime);
	FRenderLayerManage::GetRenderLayerManage()->Draw((int)EMeshComponentRenderLayerType::RENDERLAYER_SELECT, DeltaTime);

#if (OPENCOMPUTEPIPELINE == 1)
	// ������ɫ��
	ComputePipeline.Draw();
#endif

	// �༭��ui
	EditorUIPipeline.Draw(DeltaTime);
	

	// �л�pso�õģ����������ô?
	DirectXPiepelineState.Draw(DeltaTime);//��������keyboard 4 5
}

void FRenderingPipeline::PostDraw(float DeltaTime)
{
	// Ŀǰ������ɶ��û��
	GeometryMap.PostDraw(DeltaTime);
	// Ŀǰ������ɶ��û��
	FRenderLayerManage::GetRenderLayerManage()->PostDraw(DeltaTime);
}