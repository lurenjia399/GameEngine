#include "CollisionSceneQuery.h"
#include "../Rendering/Core/DirectX/RenderingPipeline/Geometry/GeometryMap.h"

bool FCollisionSceneQuery::RaycastSingle(CWorld* InWorld, const XMVECTOR& OriginPoint, const XMVECTOR& Direction, const XMMATRIX& ViewInverseMatrix, FHitResult& HitResult)
{
	for (std::weak_ptr<FGeometryDescData> data_weak : FGeometry::MeshRenderingDataPool)
	{
		auto GeometryDescData = data_weak.lock();

		float time = 0.f;
		if (GeometryDescData->AABB_box.Intersects(OriginPoint, Direction, time))
		{

		}
	}

    return false;
}
