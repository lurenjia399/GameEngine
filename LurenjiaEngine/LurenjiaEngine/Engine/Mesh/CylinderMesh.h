#pragma once
#include "Core/Mesh.h"

class CCylinderMesh : public CMesh
{
	typedef  CMesh Super;
public:
	virtual void Init() override;
	virtual void Draw(float DeltaTime) override;

	static CCylinderMesh* CreateMesh(float InTopRadius, float InBottomRadius, uint32_t InAxialSubdivision,float InHeight, uint32_t InHeightSubdivision);
private:
	virtual void BuildMesh(const FMeshRenderingData* InRenderingData) override;
};