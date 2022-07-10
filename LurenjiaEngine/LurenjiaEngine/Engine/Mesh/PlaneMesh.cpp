#include "PlaneMesh.h"
#include "../Core/CoreObject/CoreMinimalObject.h"
#include "../Mesh/Core/MeshManage.h"
#include "../Component/Mesh/PlaneMeshComponent.h"
#include "../Core/Construction/MacroConstruction.h"

void APlaneMesh::Init()
{
	Super::Init();
}

void APlaneMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);
}

void APlaneMesh::SetMeshComponent(string InName, const float& InHeight, const float& InWidth, const uint32_t& InHeightSubdivide, const uint32_t& InWidthSubdivide, EMeshComponentRenderLayerType InType)
{
	//���¸�ֵmeshcomponent���ڹ���amesh��ʱ��ʹ�����meshComponent
	//���meshComponent��ֵ��������delete��
	if (MeshComponent) delete MeshComponent;
	MeshComponent = LurenjiaEngine::CreateObject<CPlaneMeshComponent>(InName);
	MeshComponent->SetMeshComponentLayerType(InType);

	//��MeshComponetע�����ݣ�ʹ��ǰ���뱣֤MeshComponet�Ѿ��������
	//Ҳ������Ҫ��֤meshComponent����Ĳ������趨�ã����磬�㼶�ȣ�
	INJECT_DATA_INTO_MESHCOMPONENT(CPlaneMeshComponent, InHeight, InWidth, InHeightSubdivide, InWidthSubdivide);
}

void APlaneMesh::BuildMesh(const FMeshRenderingData* InRenderingData)
{
	Super::BuildMesh(InRenderingData);
}
