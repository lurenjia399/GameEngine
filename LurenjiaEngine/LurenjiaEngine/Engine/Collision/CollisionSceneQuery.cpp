#include "CollisionSceneQuery.h"
#include "../Rendering/Core/DirectX/RenderingPipeline/Geometry/GeometryMap.h"

bool FCollisionSceneQuery::RaycastSingle(CWorld* InWorld, const XMVECTOR& ViewOriginPoint, const XMVECTOR& ViewDirection, const XMMATRIX& World2ViewMatrixInverse, FHitResult& HitResult)
{
	for (std::weak_ptr<FGeometryDescData> data_weak : FGeometry::MeshRenderingDataPool)
	{
		auto GeometryDescData = data_weak.lock();

		XMMATRIX Local2WorldMatrix = XMLoadFloat4x4(&GeometryDescData->WorldMatrix);// 这个世界矩阵是主行的
		XMVECTOR Local2WorldMatrixDeterminant = DirectX::XMMatrixDeterminant(Local2WorldMatrix);
		XMMATRIX Local2WorldMatrixInverse = DirectX::XMMatrixInverse(&Local2WorldMatrixDeterminant, Local2WorldMatrix); // 求出世界变矩阵的逆

		// 注意这个顺序，这个顺序对么？ 摄像机矩阵的逆 * 世界矩阵的逆 ,,,存储的矩阵长相是一样的，但是代码这是主行，shader是主列
		XMMATRIX View2LocalMatrix = XMMatrixMultiply(World2ViewMatrixInverse, Local2WorldMatrix);

		XMVECTOR LocalOriginPoint = DirectX::XMVector3TransformCoord(ViewOriginPoint, View2LocalMatrix);
		XMVECTOR LocalDirection = DirectX::XMVector3TransformNormal(ViewDirection, View2LocalMatrix);

		float time = 0.f;
		if (GeometryDescData->AABB_box.Intersects(LocalOriginPoint, LocalDirection, time))
		{

		}
	}

    return false;
}
