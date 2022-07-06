#pragma once

#include "../../Mesh/Core/MeshManage.h"

namespace LurenjiaEngine
{
	/// <summary>
	/// ͨ��ע�����ݣ����MeshComponet�Ĵ���
	/// </summary>
	template<typename T, typename ...ParamTypes>
	T* CreateMeshComponet_Inner(CMeshManage* InMeshManage, T* InMeshComponent, ParamTypes&&... Params)
	{
		if (!InMeshManage || !InMeshComponent) return nullptr;

		size_t HashKey = 0;
		InMeshComponent->BuildKey(HashKey, std::forward<ParamTypes>(Params)...);

		FGeometryDescData GeometryDescData;
		if (InMeshManage->GetRenderingPiepeline().FindMeshRenderingDataByHash(HashKey, GeometryDescData, (int)InMeshComponent->GetMeshComponentLayerType()))
		{
			InMeshManage->GetRenderingPiepeline().DuplicateMeshRenderingData(InMeshComponent, GeometryDescData);
		}
		else {
			FMeshRenderingData MeshRenderingData;
			InMeshComponent->CreateMeshRenderData(MeshRenderingData, std::forward<ParamTypes>(Params)...);
			InMeshManage->GetRenderingPiepeline().BuildMeshComponent(InMeshComponent, MeshRenderingData, HashKey);
		}

		InMeshComponent->Init();
		return InMeshComponent;
	}

	/// <summary>
	/// ����MEshComponent����ע�����ݣ������MeshComponent�Ĵ���
	/// </summary>
	template<typename T, typename S, typename ...ParamTypes>
	T* CreateMeshComponet_Inner(CMeshManage* InMeshManage, const S& name, ParamTypes&&... Params)
	{
		if (!InMeshManage) return nullptr;

		T* meshComponent = new T();
		meshComponent->ResetGuid(name);

		return CreateMeshComponet_Inner<T>(InMeshManage, meshComponent, Params...);
	}
}