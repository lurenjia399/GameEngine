#pragma once

#include "../../Actor/Mesh/Core/MeshManage.h"

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

	/// <summary>
	/// 通过注入数据，完成MeshComponet的创建
	/// </summary>
	template<typename T, typename ...ParamTypes>
	std::shared_ptr<T> CreateMeshComponetRenderData_Inner(CMeshManage* InMeshManage, T* InMeshComponent, ParamTypes&&... Params)
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
	//std::shared_ptr<T> CreateMeshComponetRenderData_Inner(CMeshManage* InMeshManage, const S& name, ParamTypes&&... Params)
	//{
	//	std::shared_ptr<CMeshManage> InMeshManage_shared = static_pointer_cast<CMeshManage>(InMeshManage->shared_from_this());

	//	std::shared_ptr<T> meshComponent = std::make_shared<T>();
	//	meshComponent->ResetGuid(name);
	//	meshComponent->SetOuter(InMeshManage);
	//	

	//	return CreateMeshComponetRenderData_Inner<T>(InMeshManage, meshComponent, Params...);
	//}

	/// <summary>
	/// 创建MEshComponent并且注入数据，来完成MeshComponent的创建
	/// CMeshManage.cpp里会走这些
	/// </summary>
	template<typename T, typename S, typename ...ParamTypes>
	std::shared_ptr<T> CreateMeshComponetRenderData_Inner(const FCreateObjectParam& InObjectParam, CMeshManage* InMeshManage, const S& name, ParamTypes&&... Params)
	{
		if (InMeshManage)
		{
			T* MeshComponet = CreateObject<T>(InObjectParam, name);//NewObject
		}


		return CreateMeshComponetRenderData_Inner<T>(InMeshManage, MeshComponet, Params...);
	}
}