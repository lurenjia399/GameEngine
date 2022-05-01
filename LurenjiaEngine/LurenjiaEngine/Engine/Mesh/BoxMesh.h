#pragma once
#include "Core/Mesh.h"

class ABoxMesh : public AMesh
{
	typedef AMesh Super;
public:

	virtual void Init() override;
	virtual void Draw(float DeltaTime) override;

	void CreateMeshRenderData(FMeshRenderingData& InRenderingData, const float& Inheight, const float& Inwidth, const float& Indepth);
private:
	virtual void BuildMesh(const FMeshRenderingData* InRenderingData) override;
};