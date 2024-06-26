#pragma once
#include "Core/Mesh.h"

class APlaneMesh : public AMesh
{
	typedef AMesh Super;
public:

	virtual void Init() override;
	virtual void Draw(float DeltaTime) override;

	void SetMeshComponent(string InName, const float& InHeight, const float& InWidth, 
		const uint32_t& InHeightSubdivide, const uint32_t& InWidthSubdivide, 
		EMeshComponentRenderLayerType InType = EMeshComponentRenderLayerType::RENDERLAYER_OPAQUE);
private:
	virtual void BuildMesh(const FVertexRenderingData* InRenderingData) override;
};