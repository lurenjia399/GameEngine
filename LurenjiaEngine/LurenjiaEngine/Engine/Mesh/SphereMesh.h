#pragma once
#include "Core/Mesh.h"

class ASphereMesh : public AMesh
{
	typedef AMesh Super;
public:
	virtual void Init() override;
	virtual void Draw(float DeltaTime) override;
private:
	virtual void BuildMesh(const FMeshRenderingData* InRenderingData) override;
};