#pragma once
#include "Core/Mesh.h"
#include "../Component/Mesh/Core/MeshComponentType.h"

class ASphereMesh : public AMesh
{
	typedef AMesh Super;
public:
	virtual void Init() override;
	virtual void Draw(float DeltaTime) override;
	/// <summary>
	/// ע��������Ⱦ�㼶��Ĭ���ǲ�͸��(opaque)
	/// </summary>
	void SetMeshComponent(string InName, const float& InRadius, const uint32_t& InAxialSubdivision, const uint32_t& InHeightSubdivision, EMeshComponentRenderingLayerType InType = EMeshComponentRenderingLayerType::RENDERLAYER_OPAQUE);
private:
	virtual void BuildMesh(const FMeshRenderingData* InRenderingData) override;
};