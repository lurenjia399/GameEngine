#pragma once



#ifndef MeshComponentH
#define MeshComponentH

#include "../../TransformationComponent.h"
#include "../../../Mesh/Core/Material/Material.h"
#include "../../../Mesh/Core/MeshType.h"
#include "../../../Rendering/Core/DirectX/RenderingPipeline/RenderingLayer/Core/EMeshComponentRenderLayerType.h"

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

	bool GetbIsDynamicReflection() const;

	void SetMeshComponentLayerType(EMeshComponentRenderLayerType InType);
	EMeshComponentRenderLayerType GetMeshComponentLayerType();
private:
	CVARIABLE()
	vector<CMaterial*> Materials;

	EMeshComponentRenderLayerType MeshCompLayerType;
};




#endif
