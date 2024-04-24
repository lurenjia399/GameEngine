#include "CylinderMesh.h"
#include "../../Component/Mesh/CylinderMeshComponent.h"
#include "../../Core/Construction/MacroConstruction.h"

void ACylinderMesh::Init()
{
	Super::Init();
}

void ACylinderMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);
}

void ACylinderMesh::SetMeshComponent(string InName, const float& InTopRadius, const float& InBottomRadius, const uint32_t& InAxialSubdivision, const float& InHeight, const uint32_t& InHeightSubdivision, EMeshComponentRenderLayerType InType)
{
	//���¸�ֵmeshcomponent���ڹ���amesh��ʱ��ʹ�����meshComponent
	//���meshComponent��ֵ��������delete��
	if (MeshComponent.get()) MeshComponent.reset();
	MeshComponent = LurenjiaEngine::CreateObject<CCylinderMeshComponent>(this, InName);
	MeshComponent->SetMeshComponentLayerType(InType);

	//��MeshComponetע�����ݣ�ʹ��ǰ���뱣֤MeshComponet�Ѿ��������
	//Ҳ������Ҫ��֤meshComponent����Ĳ������趨�ã����磬�㼶�ȣ�
	InjectComponentRenderDataByMesh(CCylinderMeshComponent, InTopRadius, InBottomRadius, InAxialSubdivision, InHeight, InHeightSubdivision);
}

void ACylinderMesh::BuildMesh(const FVertexRenderingData* InRenderingData)
{
	Super::BuildMesh(InRenderingData);
}
