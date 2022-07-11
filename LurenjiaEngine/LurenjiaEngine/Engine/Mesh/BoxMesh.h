#pragma once
#include "Core/Mesh.h"

class ABoxMesh : public AMesh
{
	typedef AMesh Super;
public:

	virtual void Init() override;
	virtual void Draw(float DeltaTime) override;
	void SetMeshComponent(string InName, const float& Inheight, const float& Inwidth, const float& Indepth, EMeshComponentRenderLayerType InType = EMeshComponentRenderLayerType::RENDERLAYER_OPAQUE);
private:
	virtual void BuildMesh(const FMeshRenderingData* InRenderingData) override;
};