#pragma once

#include "../../Actor/Mesh/Core/MeshManage.h"

namespace LurenjiaEngine
{

	/// <summary>
	/// 通过注入数据，完成MeshComponet的创建
	/// </summary>
	template<typename T, typename ...ParamTypes>
	T* CreateMeshComponetRenderData_Inner(CMeshManage* InMeshManage, T* InMeshComponent, ParamTypes&&... Params)
	{

		size_t HashKey = 0;
		InMeshComponent->BuildKey(HashKey, std::forward<ParamTypes>(Params)...);

		std::weak_ptr<FGeometryDescData> OutGeometryDescData;
		if (InMeshManage->GetRenderingPiepeline().FindMeshRenderingDataByHash(HashKey, OutGeometryDescData, (int)InMeshComponent->GetMeshComponentLayerType()))
		{
			InMeshManage->GetRenderingPiepeline().DuplicateMeshRenderingData(InMeshComponent, OutGeometryDescData);
		}
		else {
			// 创建component的渲染数据
			FVertexRenderingData OutMeshRenderingData;
			InMeshComponent->CreateMeshRenderData(OutMeshRenderingData, std::forward<ParamTypes>(Params)...);

			InMeshManage->GetRenderingPiepeline().BuildMeshComponent(InMeshComponent, OutMeshRenderingData, HashKey);
		}

		InMeshComponent->Init();
		return InMeshComponent;
	}

	//template<typename T, typename S, typename ...ParamTypes>
	//T* CreateMeshComponetRenderData_Inner(const FCreateObjectParam& InObjectParam, CMeshManage* InMeshManage, const S& name, ParamTypes&&... Params)
	//{
	//	if (InMeshManage)
	//	{
	//		T* MeshComponet = LurenjiaEngine::CreateObject<T>(InObjectParam, name);//NewObject
	//		return CreateMeshComponetRenderData_Inner<T>(InMeshManage, MeshComponet, Params...);
	//	}
	//}
}