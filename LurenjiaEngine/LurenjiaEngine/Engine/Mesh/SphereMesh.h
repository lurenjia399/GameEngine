#pragma once
#include "Core/Mesh.h"

class CSphereMesh : public CMesh
{
	typedef  CMesh Super;
public:
	virtual void Init() override;
	virtual void Draw(float DeltaTime) override;

	static CSphereMesh* CreateMesh();
private:
	virtual void BuildMesh(const FMeshRenderingData* InRenderingData) override;
};