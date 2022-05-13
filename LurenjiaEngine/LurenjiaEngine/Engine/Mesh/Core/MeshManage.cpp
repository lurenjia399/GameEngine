#include "MeshManage.h"
#include "../../Rendering/Engine/DirectX/Core/DirectXRenderingEngine.h"
#include "ObjectTransformation.h"
#include "../../Config/EngineRenderConfig.h"
#include "../../Rendering/Core/Buffer/ConstructBuffer.h"
#include "../BoxMesh.h"
#include "../PlaneMesh.h"
#include "../CylinderMesh.h"
#include "../CustomMesh.h"
#include "../SphereMesh.h"
#include "../../Math/EngineMath.h"
#include "../../Core/Viewport/ViewportTransformation.h"

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

AMesh* CMeshManage::CreateBoxMesh(string InName, const float& Inheight, const float& Inwidth, const float& Indepth)
{
	return CreateMesh<ABoxMesh>(InName, Inheight, Inwidth, Indepth);
}

AMesh* CMeshManage::CreatePlaneMesh(string InName, const float& Inheight, const float& Inwidth, const uint32_t& InHeightSubdivide, const uint32_t& InwidthSubdivide)
{
	return CreateMesh<APlaneMesh>(InName, Inheight, Inwidth, InHeightSubdivide, InwidthSubdivide);
}

AMesh* CMeshManage::CreateCylinderMesh(string InName, const float& InTopRadius, const float& InBottomRadius, const uint32_t& InAxialSubdivision, const float& InHeight, const uint32_t& InHeightSubdivision)
{
	return CreateMesh<ACylinderMesh>(InName, InTopRadius, InBottomRadius, InAxialSubdivision, InHeight, InHeightSubdivision);
}

AMesh* CMeshManage::CreateCustomMesh(string InName, const string& InPath)
{
	return CreateMesh<ACustomMesh>(InName, InPath);
}

AMesh* CMeshManage::CreateSphereMesh(string InName, const float& InRadius, const uint32_t& InAxialSubdivision, const uint32_t& InHeightSubdivision)
{
	return CreateMesh<ASphereMesh>(InName, InRadius, InAxialSubdivision, InHeightSubdivision);
}
