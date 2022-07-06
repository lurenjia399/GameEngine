#include "MeshManage.h"
#include "../../Rendering/Engine/DirectX/Core/DirectXRenderingEngine.h"
#include "ObjectTransformation.h"
#include "../../Config/EngineRenderConfig.h"
#include "../../Rendering/Core/Buffer/ConstructBuffer.h"
#include "../../Component/Mesh/BoxMeshComponent.h"
#include "../../Component/Mesh/SphereMeshComponent.h"
#include "../../Component/Mesh/CylinderMeshComponent.h"
#include "../../Component/Mesh/CustomMeshComponent.h"
#include "../../Component/Mesh/PlaneMeshComponent.h"
#include "../../Math/EngineMath.h"
#include "../../Core/Viewport/ViewportTransformation.h"
#include "../../Core/Construction/MeshComponentConstruction_Inner.h"

CMeshManage::CMeshManage()
{
}

void CMeshManage::Init()
{
	
}

void CMeshManage::BuildPipeline()
{
	RenderingPipeline.BuildPipeline();
}

void CMeshManage::PreDraw(float DeltaTime)
{
	RenderingPipeline.PreDraw(DeltaTime);
}

void CMeshManage::Draw(float DeltaTime)
{
	RenderingPipeline.Draw(DeltaTime);
}

void CMeshManage::PostDraw(float DeltaTime)
{
	RenderingPipeline.PostDraw(DeltaTime);
}

void CMeshManage::UpdateConstantView(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	RenderingPipeline.UpdateConstantView(DeltaTime, ViewportInfo);
}

CMeshComponent* CMeshManage::CreateBoxMeshComponent(string InName, const float& Inheight, const float& Inwidth, const float& Indepth)
{
	return LurenjiaEngine::CreateMeshComponet_Inner<CBoxMeshComponent>(this, InName, Inheight, Inwidth, Indepth);
}

CMeshComponent* CMeshManage::CreatePlaneMeshComponent(string InName, const float& Inheight, const float& Inwidth, const uint32_t& InHeightSubdivide, const uint32_t& InwidthSubdivide)
{
	return LurenjiaEngine::CreateMeshComponet_Inner<CPlaneMeshComponent>(this, InName, Inheight, Inwidth, InHeightSubdivide, InwidthSubdivide);
}

CMeshComponent* CMeshManage::CreateCylinderMeshComponent(string InName, const float& InTopRadius, const float& InBottomRadius, const uint32_t& InAxialSubdivision, const float& InHeight, const uint32_t& InHeightSubdivision)
{
	return LurenjiaEngine::CreateMeshComponet_Inner<CCylinderMeshComponent>(this, InName, InTopRadius, InBottomRadius, InAxialSubdivision, InHeight, InHeightSubdivision);
}

CMeshComponent* CMeshManage::CreateCustomMeshComponent(string InName, const string& InPath)
{
	return LurenjiaEngine::CreateMeshComponet_Inner<CCustomMeshComponent>(this, InName, InPath);
}

CMeshComponent* CMeshManage::CreateSphereMeshComponent(string InName, const float& InRadius, const uint32_t& InAxialSubdivision, const uint32_t& InHeightSubdivision)
{
	return LurenjiaEngine::CreateMeshComponet_Inner<CSphereMeshComponent>(this, InName, InRadius, InAxialSubdivision, InHeightSubdivision);
}

FRenderingPipeline& CMeshManage::GetRenderingPiepeline()
{
	return RenderingPipeline;
}
