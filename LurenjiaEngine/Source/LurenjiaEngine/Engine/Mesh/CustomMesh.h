#pragma once
#include "Core/Mesh.h"

class ACustomMesh : public AMesh
{
	typedef AMesh Super;
public:

	virtual void Init() override;
	virtual void Draw(float DeltaTime) override;

	/// <summary>
	/// ע��������Ⱦ�㼶��Ĭ���ǲ�͸��(opaque)
	/// </summary>
	void SetMeshComponent(string InName, const string& InPath, EMeshComponentRenderLayerType InType = EMeshComponentRenderLayerType::RENDERLAYER_OPAQUE);

private:
	virtual void BuildMesh(const FMeshRenderingData* InRenderingData) override;
};