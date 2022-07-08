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
	virtual void Init() override;//��д���ി�麯��
	virtual void PreDraw(float DeltaTime) override;
	virtual void Draw(float DeltaTime) override;//��д���ി�麯��
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
	//��ȡ����任���
	void SetComponentPosition(const XMFLOAT3& InPosition);
	void SetComponentRotation(const fvector_3d& InRotation);
	void SetComponentScale(const XMFLOAT3& InScale);
public:
	//��ȡ����������
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