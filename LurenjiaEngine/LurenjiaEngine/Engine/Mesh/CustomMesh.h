#pragma once
#include "Core/Mesh.h"

class CCustomMesh : public CMesh
{
	typedef CMesh Super;
public:

	virtual void Init() override;
	virtual void Draw(float DeltaTime) override;

	void CreateMeshRenderData(FMeshRenderingData& InRenderingData, const string& InPath);

private:
	virtual void BuildMesh(const FMeshRenderingData* InRenderingData) override;
	static bool LoadObjFileBuffer(char* InBuffer, uint32_t InBufferSize, FMeshRenderingData& InRenderingData);
};