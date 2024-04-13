#pragma once



#ifndef MeshComponentH
#define MeshComponentH

#include "../../TransformationComponent.h"
#include "../../../Actor/Mesh/Core/Material/Material.h"
#include "../../../Actor/Mesh/Core/MeshType.h"
#include "../../../Rendering/Core/DirectX/RenderingPipeline/RenderingLayer/Core/EMeshComponentRenderLayerType.h"

class CMeshComponent : public CTransformationComponent
{
	typedef CTransformationComponent Super;
public:
	CMeshComponent();
public:
	void Init();
	void BuildMesh(const FVertexRenderingData* InRenderingData);

	void SetSubMaterials(const int& index, shared_ptr<CMaterial> InMaterial);
	UINT GetMaterialsCount() const;
	const vector<shared_ptr<CMaterial>> GetMaterials() const;

	bool GetbIsDynamicReflection() const;

	void SetMeshComponentLayerType(EMeshComponentRenderLayerType InType);
	EMeshComponentRenderLayerType GetMeshComponentLayerType();

	void SetPickup(bool bNewPickup);
	bool GetIsPickup() const;
protected:
	bool bPickup;
private:
	CVARIABLE()
	vector<shared_ptr<CMaterial>> Materials;

	EMeshComponentRenderLayerType MeshCompLayerType;
};




#endif
