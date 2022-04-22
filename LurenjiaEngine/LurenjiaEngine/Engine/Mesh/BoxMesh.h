#pragma once
#include "Core/Mesh.h"

class CBoxMesh : public CMesh
{
	typedef CMesh Super;
public:

	virtual void Init() override;
	virtual void Draw(float DeltaTime) override;

	void CreateMeshRenderData(FMeshRenderingData& InRenderingData, const float& Inheight, const float& Inwidth, const float& Indepth);
private:
	virtual void BuildMesh(const FMeshRenderingData* InRenderingData) override;
};