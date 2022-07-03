#pragma once



#ifndef MeshComponentH
#define MeshComponentH

#include "../../TransformationComponent.h"
#include "../../../Mesh/Core/Material/Material.h"
#include "../../../Mesh/Core/MeshType.h"
#include "MeshComponentType.h"

class CMeshComponent : public CTransformationComponent
{
public:
	CMeshComponent();
public:
	void Init();
	void BuildMesh(const FMeshRenderingData* InRenderingData);

	void SetSubMaterials(const int& index, CMaterial* InMaterial);
	UINT GetMaterialsCount() const;
	const vector<CMaterial*>* GetMaterials() const;

	void SetMeshComponentLayerType(EMeshComponentRenderingLayerType InType);
	EMeshComponentRenderingLayerType GetMeshComponentLayerType();
private:
	CVARIABLE()
	vector<CMaterial*> Materials;

	EMeshComponentRenderingLayerType MeshCompLayerType;
};




#endif
