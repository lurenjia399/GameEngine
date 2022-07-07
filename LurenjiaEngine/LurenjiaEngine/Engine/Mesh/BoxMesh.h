#pragma once
#include "Core/Mesh.h"

class ABoxMesh : public AMesh
{
	typedef AMesh Super;
public:

	virtual void Init() override;
	virtual void Draw(float DeltaTime) override;
	void SetMeshComponent(string InName, const float& InRadius, const uint32_t& InAxialSubdivision, const uint32_t& InHeightSubdivision);
private:
	virtual void BuildMesh(const FMeshRenderingData* InRenderingData) override;
};