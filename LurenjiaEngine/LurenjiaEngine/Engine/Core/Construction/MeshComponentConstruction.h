#pragma once

#include "MeshComponentConstruction_Inner.h"
#include "../../Mesh/Core/Mesh.h"

namespace LurenjiaEngine
{
	/// <summary>
	/// ʹ���������ʱ�����뱣֤����InMesh������Ӧ��MeshComponet
	/// </summary>
	/// <typeparam name="T">������MeshComponet����</typeparam>
	/// <typeparam name="...ParamTypes">MeshComponet����Ĳ�������</typeparam>
	/// <param name="InMeshManage">ģ�͹�����</param>
	/// <param name="InMesh">ģ��</param>
	/// <param name="...Params">����</param>
	/// <returns></returns>
	template<typename T, typename ...ParamTypes>
	T* CreateMeshComponet(CMeshManage* InMeshManage, AMesh* InMesh, ParamTypes&&... Params)
	{
		return CreateMeshComponet_Inner<T>(InMeshManage, InMesh->GetMeshComponet<T>(), Params...);
	}
}