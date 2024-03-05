#include "CustomMesh.h"
#include "../Core/CoreObject/CoreMinimalObject.h"
#include "../Mesh/Core/MeshManage.h"
#include "../Core/Construction/MacroConstruction.h"
#include "../Component/Mesh/CustomMeshComponent.h"

void ACustomMesh::Init()
{
	Super::Init();
}

void ACustomMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);
}

void ACustomMesh::SetMeshComponent(string InName, const string& InPath, EMeshComponentRenderLayerType InType)
{
	//���¸�ֵmeshcomponent���ڹ���amesh��ʱ��ʹ�����meshComponent
	//���meshComponent��ֵ��������delete��
	if (MeshComponent.get()) MeshComponent.reset();
	MeshComponent = LurenjiaEngine::CreateObject<CCustomMeshComponent>(this, InName);
	MeshComponent->SetMeshComponentLayerType(InType);



	INJECT_DATA_INTO_MESHCOMPONENT(CCustomMeshComponent, InPath);
}

void ACustomMesh::BuildMesh(const FMeshRenderingData* InRenderingData)
{
	Super::BuildMesh(InRenderingData);
}
