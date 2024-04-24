#pragma once

#include "../../Actor/Mesh/Core/MeshManage.h"

namespace LurenjiaEngine
{
	/// <summary>
	/// ͨ��ע�����ݣ����MeshComponet�Ĵ���
	/// </summary>
	template<typename T, typename ...ParamTypes>
	std::shared_ptr<T> CreateMeshComponetRenderData_Inner(shared_ptr<CMeshManage> InMeshManage, std::shared_ptr<T> InMeshComponent, ParamTypes&&... Params)
	{
		if (InMeshManage.use_count() == 0) return nullptr;

		size_t HashKey = 0;
		InMeshComponent->BuildKey(HashKey, std::forward<ParamTypes>(Params)...);

		std::weak_ptr<FGeometryDescData> OutGeometryDescData;
		if (InMeshManage->GetRenderingPiepeline().FindMeshRenderingDataByHash(HashKey, OutGeometryDescData, (int)InMeshComponent->GetMeshComponentLayerType()))
		{
			InMeshManage->GetRenderingPiepeline().DuplicateMeshRenderingData(InMeshComponent, OutGeometryDescData);
		}
		else {
			FVertexRenderingData MeshRenderingData;
			InMeshComponent->CreateMeshRenderData(MeshRenderingData, std::forward<ParamTypes>(Params)...);
			InMeshManage->GetRenderingPiepeline().BuildMeshComponent(InMeshComponent, MeshRenderingData, HashKey);
		}

		InMeshComponent->Init();
		return InMeshComponent;
	}

	/// <summary>
	/// ����MEshComponent����ע�����ݣ������MeshComponent�Ĵ���
	/// CMeshManage.cpp�������Щ
	/// </summary>
	template<typename T, typename S, typename ...ParamTypes>
	std::shared_ptr<T> CreateMeshComponetRenderData_Inner(CMeshManage* InMeshManage, const S& name, ParamTypes&&... Params)
	{
		std::shared_ptr<CMeshManage> InMeshManage_shared = static_pointer_cast<CMeshManage>(InMeshManage->shared_from_this());

		std::shared_ptr<T> meshComponent = std::make_shared<T>();
		meshComponent->ResetGuid(name);
		meshComponent->SetOuter(InMeshManage);
		

		return CreateMeshComponetRenderData_Inner<T>(InMeshManage_shared, meshComponent, Params...);
	}
}