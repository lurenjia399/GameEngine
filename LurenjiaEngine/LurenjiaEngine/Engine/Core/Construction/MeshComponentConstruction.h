#pragma once

#include "MeshComponentConstruction_Inner.h"
#include "../../Mesh/Core/Mesh.h"

namespace LurenjiaEngine
{
	/// <summary>
	/// 使用这个方法时，必须保证参数InMesh上有相应的MeshComponet
	/// </summary>
	/// <typeparam name="T">创建的MeshComponet类型</typeparam>
	/// <typeparam name="...ParamTypes">MeshComponet所需的参数类型</typeparam>
	/// <param name="InMeshManage">模型管理器</param>
	/// <param name="InMesh">模型</param>
	/// <param name="...Params">参数</param>
	/// <returns></returns>
	template<typename T, typename ...ParamTypes>
	T* CreateMeshComponet(CMeshManage* InMeshManage, AMesh* InMesh, ParamTypes&&... Params)
	{
		return CreateMeshComponet_Inner<T>(InMeshManage, InMesh->GetMeshComponet<T>(), Params...);
	}
}