#include "BoxMesh.h"
#include "../Core/CoreObject/CoreMinimalObject.h"
#include "../Core/Construction/MacroConstruction.h"
#include "../Mesh/Core/MeshManage.h"
#include "../Component/Mesh/BoxMeshComponent.h"

void ABoxMesh::Init()
{
	Super::Init();
}

void ABoxMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);
}

void ABoxMesh::SetMeshComponent(string InName, const float& Inheight, const float& Inwidth, const float& Indepth, EMeshComponentRenderLayerType InType)
{
	//���¸�ֵmeshcomponent���ڹ���amesh��ʱ��ʹ�����meshComponent
	//���meshComponent��ֵ��������delete��
	if (MeshComponent.get()) MeshComponent.reset();
	MeshComponent = LurenjiaEngine::CreateObject<CBoxMeshComponent>(this, InName);
	MeshComponent->SetMeshComponentLayerType(InType);

	INJECT_DATA_INTO_MESHCOMPONENT(CBoxMeshComponent, Inheight, Inwidth, Indepth);
}

void ABoxMesh::BuildMesh(const FMeshRenderingData* InRenderingData)
{
	Super::BuildMesh(InRenderingData);
}
