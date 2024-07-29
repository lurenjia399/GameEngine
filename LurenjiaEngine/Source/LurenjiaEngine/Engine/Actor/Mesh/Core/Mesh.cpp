#include "Mesh.h"
#include "Material/Material.h"


AMesh::AMesh()
	: MeshComponent(nullptr)
{
	//MeshComponent->ResetGuid("AMesh::MeshComponent");
}

void AMesh::Init()
{
	if (MeshComponent)
	{
		MeshComponent->Init();
	}
}

void AMesh::PreDraw(float DeltaTime)
{
	
}

void AMesh::Draw(float DeltaTime)
{
	
}

void AMesh::PostDraw(float DeltaTime)
{
	
}

void AMesh::SetPosition(const XMFLOAT3& InPosition)
{
	if (RootComponent)
	{
		RootComponent->SetPosition(InPosition);
	}
	else
	{
		MeshComponent->SetPosition(InPosition);
	}
	
}

void AMesh::SetRotation(const fvector_3d& InRotation)
{
	if (RootComponent)
	{
		RootComponent->SetRotation(InRotation);
	}
	else
	{
		MeshComponent->SetRotation(InRotation);
	}
}

void AMesh::SetScale(const XMFLOAT3& InScale)
{
	if (RootComponent)
	{
		RootComponent->SetScale(InScale);
	}
	else
	{
		MeshComponent->SetScale(InScale);
	}
}

void AMesh::BuildMesh(const FVertexRenderingData* InRenderingData)
{
	if (MeshComponent)
	{
		MeshComponent->BuildMesh(InRenderingData);
	}
}

void AMesh::SetPickup(bool bNewPickup)
{
	MeshComponent->SetPickup(bNewPickup);
}

void AMesh::SetVisible(bool InVisible)
{
	if (RootComponent)
	{
		RootComponent->SetVisible(InVisible);
	}
	else
	{
		MeshComponent->SetVisible(InVisible);
	}
}

bool AMesh::GetVisible()
{
	return RootComponent->GetIsVisible();
}

void AMesh::SetMeshComponent(CMeshComponent* InMeshComponent)
{
	MeshComponent = InMeshComponent;

	MeshComponent->AttachToComponent(RootComponent);
}

void AMesh::SetMeshComponentLayerType(EMeshComponentRenderLayerType InType)
{
	MeshComponent->SetMeshComponentLayerType(InType);
}

void AMesh::SetComponentPosition(const XMFLOAT3& InPosition)
{
	MeshComponent->SetPositionOffset(InPosition);
	MeshComponent->UpdateComponentPositionToWorldWithParent(MeshComponent->GetAttachParent(), InPosition);
}

void AMesh::SetComponentRotation(const fvector_3d& InRotation)
{
	MeshComponent->UpdateComponentRotationToWorldWithParent(MeshComponent->GetAttachParent(), InRotation);
}

void AMesh::SetComponentScale(const XMFLOAT3& InScale)
{
	MeshComponent->UpdateComponentScaleToWorldWithParent(MeshComponent->GetAttachParent(), InScale);
}

void AMesh::SetSubMaterials(const int& index, shared_ptr<CMaterial> InMaterial)
{
	MeshComponent->SetSubMaterials(index, InMaterial);
}

UINT AMesh::GetMaterialsCount() const
{
	return MeshComponent->GetMaterialsCount();
}

const vector<shared_ptr<CMaterial>> AMesh::GetMaterials() const
{
	return MeshComponent->GetMaterials();
}



