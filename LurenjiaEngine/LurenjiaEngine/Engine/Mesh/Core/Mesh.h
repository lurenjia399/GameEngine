#pragma once
#include "../../Rendering/Core/Rendering.h"
#include "MeshType.h"
#include "../../Shader/Core/Shader.h"

class CMesh :public CCoreMinimalObject, public IRenderingInterface
{
public:
	CMesh();
	virtual void Init() override;//��д���ി�麯��
	virtual void PreDraw(float DeltaTime) override;
	virtual void Draw(float DeltaTime) override;//��д���ി�麯��
	virtual void PostDraw(float DeltaTime) override;

	virtual void BuildMesh(const FMeshRenderingData* InRenderingData);

};