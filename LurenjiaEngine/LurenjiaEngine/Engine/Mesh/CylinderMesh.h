#pragma once
#include "Core/Mesh.h"

class ACylinderMesh : public AMesh
{
	typedef  AMesh Super;
public:
	virtual void Init() override;
	virtual void Draw(float DeltaTime) override;

	void SetMeshComponent(string InName, const float& InTopRadius, const float& InBottomRadius, const uint32_t& InAxialSubdivision, const float& InHeight, const uint32_t& InHeightSubdivision);
private:
	virtual void BuildMesh(const FMeshRenderingData* InRenderingData) override;
};