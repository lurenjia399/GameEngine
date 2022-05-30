#pragma once
#include "../../Rendering/Core/Rendering.h"
#include "MeshType.h"
#include "../../Shader/Core/Shader.h"
#include "../../Component/TransformationComponent.h"
#include "../../Actor/Core/Actor.h"
#include "../../Component/Mesh/Core/MeshComponent.h"
#include "../../Component/Mesh/StaticMeshComponent.h"


class CMaterial;
class AMesh :public AActor, public IRenderingInterface
{
public:
	AMesh();
	virtual void Init() override;//重写父类纯虚函数
	virtual void PreDraw(float DeltaTime) override;
	virtual void Draw(float DeltaTime) override;//重写父类纯虚函数
	virtual void PostDraw(float DeltaTime) override;

	virtual void BuildMesh(const FMeshRenderingData* InRenderingData);
public:
	CStaticMeshComponent* GetMeshComponet() { return StaticMeshComponent; }
public:
	void SetSubMaterials(const int& index, CMaterial* InMaterial);
	UINT GetMaterialsCount() const;
	const vector<CMaterial*>* GetMaterials() const;
private:
	CStaticMeshComponent* StaticMeshComponent;
	
};