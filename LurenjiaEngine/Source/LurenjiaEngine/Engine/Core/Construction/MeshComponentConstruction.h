#pragma once

#include "MeshComponentConstruction_Inner.h"
#include "../../Actor/Mesh/Core/Mesh.h"

namespace LurenjiaEngine
{

	template<typename T, typename ...ParamTypes>
	T* CreateMeshComponetRenderDataByMesh(CMeshManage* InMeshManage, AMesh* InMesh, ParamTypes&&... Params)
	{
		return CreateMeshComponetRenderData_Inner<T>(InMeshManage, InMesh->GetMeshComponet<T>(), Params...);
	}

	template<typename T, typename ...ParamTypes>
	T* CreateMeshComponetRenderDataByComponent(CMeshManage* InMeshManage, T* InComponent, ParamTypes&&... Params)
	{
		return CreateMeshComponetRenderData_Inner<T>(InMeshManage, InComponent, Params...);
	}
}