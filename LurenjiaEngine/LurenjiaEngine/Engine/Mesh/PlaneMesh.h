#pragma once
#include "Core/Mesh.h"

class CPlaneMesh : public CMesh
{
	typedef CMesh Super;
public:

	virtual void Init() override;
	virtual void Draw(float DeltaTime) override;

	static CPlaneMesh* CreateMesh(float Inheight, float Inwidth, uint32_t InHeightSubdivide, uint32_t InwidthSubdivide);
private:
	virtual void BuildMesh(const FMeshRenderingData* InRenderingData) override;
};