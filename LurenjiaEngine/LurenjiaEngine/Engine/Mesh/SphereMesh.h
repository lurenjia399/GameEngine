#pragma once
#include "Core/Mesh.h"

class CSphereMesh : public CMesh
{
	typedef  CMesh Super;
public:
	virtual void Init() override;
	virtual void Draw(float DeltaTime) override;

	void CreateMeshRenderData(FMeshRenderingData& InRenderingData, const float& InRadius, const uint32_t& InAxialSubdivision, const uint32_t& InHeightSubdivision);
private:
	virtual void BuildMesh(const FMeshRenderingData* InRenderingData) override;
};