#pragma once

#include "MeshComponentConstruction_Inner.h"
#include "../../Actor/Mesh/Core/Mesh.h"

namespace LurenjiaEngine
{

	template<typename T, typename ...ParamTypes>
	std::shared_ptr<T> CreateMeshComponetRenderDataByMesh(shared_ptr<CMeshManage> InMeshManage, AMesh* InMesh, ParamTypes&&... Params)
	{
		return CreateMeshComponetRenderData_Inner<T>(InMeshManage, InMesh->GetMeshComponet<T>(), Params...);
	}

	template<typename T, typename ...ParamTypes>
	std::shared_ptr<T> CreateMeshComponetRenderDataByComponent(CMeshManage* InMeshManage, shared_ptr<T> InComponent, ParamTypes&&... Params)
	{
		return CreateMeshComponetRenderData_Inner<T>(InMeshManage, InComponent, Params...);
	}
}