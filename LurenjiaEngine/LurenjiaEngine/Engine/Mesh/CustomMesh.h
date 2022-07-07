#pragma once
#include "Core/Mesh.h"

class ACustomMesh : public AMesh
{
	typedef AMesh Super;
public:

	virtual void Init() override;
	virtual void Draw(float DeltaTime) override;

	void SetMeshComponent(string InName, const string& InPath);

private:
	virtual void BuildMesh(const FMeshRenderingData* InRenderingData) override;
};