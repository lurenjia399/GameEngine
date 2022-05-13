#pragma once
#include "../../Rendering/Core/Rendering.h"
#include "MeshType.h"
#include "../../Shader/Core/Shader.h"
#include "../../Component/TransformationComponent.h"
#include "../../Actor/Core/Actor.h"


class CMaterial;
class AMesh :public AActor, public IRenderingInterface
{
public:
	AMesh();
	virtual void Init() override;//��д���ി�麯��
	virtual void PreDraw(float DeltaTime) override;
	virtual void Draw(float DeltaTime) override;//��д���ി�麯��
	virtual void PostDraw(float DeltaTime) override;

	virtual void BuildMesh(const FMeshRenderingData* InRenderingData);

public:
	UINT GetMaterialsCount() const;
private:
	CVARIABLE()
	vector<CMaterial*> Materials;

};