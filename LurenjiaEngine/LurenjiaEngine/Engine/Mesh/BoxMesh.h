#pragma once
#include "Core/Mesh.h"

class CBoxMesh : public CMesh
{
	typedef CMesh Super;
public:

	virtual void Init() override;
	virtual void Draw(float DeltaTime) override;

	static CBoxMesh* CreateMesh();
private:
	virtual void BuildMesh(const FMeshRenderingData* InRenderingData) override;
};