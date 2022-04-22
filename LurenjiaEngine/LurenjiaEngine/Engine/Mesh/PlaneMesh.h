#pragma once
#include "Core/Mesh.h"

class CPlaneMesh : public CMesh
{
	typedef CMesh Super;
public:

	virtual void Init() override;
	virtual void Draw(float DeltaTime) override;

	void CreateMeshRenderData(FMeshRenderingData& InRenderingData, const float& Inheight, const float& Inwidth, const uint32_t& InHeightSubdivide, const uint32_t& InwidthSubdivide);
private:
	virtual void BuildMesh(const FMeshRenderingData* InRenderingData) override;
};