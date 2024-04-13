#include "SphereMesh.h"
#include "../../Core/CoreObject/CoreMinimalObject.h"
#include "../Mesh/Core/MeshManage.h"
#include "../../Core/Construction/MacroConstruction.h"
#include "../../Component/Mesh/SphereMeshComponent.h"

void ASphereMesh::Init()
{
	Super::Init();
}

void ASphereMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);
}

void ASphereMesh::SetMeshComponent(string InName, const float& InRadius, const uint32_t& InAxialSubdivision, const uint32_t& InHeightSubdivision, EMeshComponentRenderLayerType InType)
{
	//���¸�ֵmeshcomponent���ڹ���amesh��ʱ��ʹ�����meshComponent
	//���meshComponent��ֵ��������delete��
	if (MeshComponent.get()) MeshComponent.reset();
	MeshComponent = LurenjiaEngine::CreateObject<CSphereMeshComponent>(this, InName);
	MeshComponent->SetMeshComponentLayerType(InType);

	//��MeshComponetע�����ݣ�ʹ��ǰ���뱣֤MeshComponet�Ѿ��������
	//Ҳ������Ҫ��֤meshComponent����Ĳ������趨�ã����磬�㼶�ȣ�
	INJECT_DATA_INTO_MESHCOMPONENT(CSphereMeshComponent, InRadius, InAxialSubdivision, InHeightSubdivision)

}

void ASphereMesh::BuildMesh(const FVertexRenderingData* InRenderingData)
{
	Super::BuildMesh(InRenderingData);
}
