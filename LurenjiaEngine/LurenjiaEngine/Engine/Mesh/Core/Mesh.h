#pragma once
#include "../../Rendering/Core/Rendering.h"
#include "MeshType.h"
#include "../../Shader/Core/Shader.h"
#include "../../Component/TransformationComponent.h"
#include "../../Actor/Core/Actor.h"
#include "../../Component/Mesh/Core/MeshComponent.h"
#include "../../Interface/DirectXDeviceInterface.h"


class CMaterial;
class AMesh :public AActor, public IRenderingInterface, public IDirectXDeviceInterface
{
public:
	AMesh();
	virtual void Init() override;//重写父类纯虚函数
	virtual void PreDraw(float DeltaTime) override;
	virtual void Draw(float DeltaTime) override;//重写父类纯虚函数
	virtual void PostDraw(float DeltaTime) override;

	virtual void SetPosition(const XMFLOAT3& InPosition) override;
	virtual void SetRotation(const fvector_3d& InRotation) override;
	virtual void SetScale(const XMFLOAT3& InScale) override;

	virtual void BuildMesh(const FMeshRenderingData* InRenderingData);
public:
	void SetMeshComponent(CMeshComponent* InMeshComponent);
	void SetMeshComponentLayerType(EMeshComponentRenderingLayerType InType);
	template<typename T>
	T* GetMeshComponet();
public:
	//获取组件变换相关
	void SetComponentPosition(const XMFLOAT3& InPosition);
	void SetComponentRotation(const fvector_3d& InRotation);
	void SetComponentScale(const XMFLOAT3& InScale);
public:
	//获取组件材质相关
	void SetSubMaterials(const int& index, CMaterial* InMaterial);
	UINT GetMaterialsCount() const;
	const vector<CMaterial*>* GetMaterials() const;
protected:
	CMeshComponent* MeshComponent;
};

template<typename T>
T* AMesh::GetMeshComponet()
{
	return dynamic_cast<T*>(MeshComponent);
}