#include "CollisionSceneQuery.h"
#include "../Rendering/Core/DirectX/RenderingPipeline/Geometry/GeometryMap.h"

bool FCollisionSceneQuery::RaycastSingle(shared_ptr<CWorld> InWorld, const XMVECTOR& ViewOriginPoint, const XMVECTOR& ViewDirection, const XMMATRIX& World2ViewMatrixInverse, FHitResult& OutHitResult)
{
	float FinalTime = FLT_MAX;
	// 遍历池子中的所有模型
	for (std::weak_ptr<FGeometryDescData> data_weak : FGeometry::MeshRenderingDataPool)
	{
		auto GeometryDescData = data_weak.lock();

		XMMATRIX Local2WorldMatrix = XMLoadFloat4x4(&GeometryDescData->WorldMatrix);// 这个世界矩阵是主行的
		XMVECTOR Local2WorldMatrixDeterminant = DirectX::XMMatrixDeterminant(Local2WorldMatrix);
		XMMATRIX Local2WorldMatrixInverse = DirectX::XMMatrixInverse(&Local2WorldMatrixDeterminant, Local2WorldMatrix); // 求出世界变矩阵的逆

		
		// 注意这个顺序，这个顺序对么？ 摄像机矩阵的逆 * 世界矩阵的逆 ,,,存储的矩阵长相是一样的，但是代码这是主行，shader是主列
		XMMATRIX View2LocalMatrix = XMMatrixMultiply(World2ViewMatrixInverse, Local2WorldMatrixInverse);

		XMVECTOR LocalOriginPoint = DirectX::XMVector3TransformCoord(ViewOriginPoint, View2LocalMatrix);
		XMVECTOR LocalDirection = DirectX::XMVector3TransformNormal(ViewDirection, View2LocalMatrix);
		LocalDirection = DirectX::XMVector3Normalize(LocalDirection);

		float BoundTime = 0.f; //碰撞到AABB的时间
		float TriangleTime = FLT_MAX;
		// 找到和模型AABB有交点的模型
		if (GeometryDescData->AABB_box.Intersects(LocalOriginPoint, LocalDirection, BoundTime))
		{
			if (BoundTime > 0)
			{
				if (BoundTime < FinalTime)
				{
					if (GeometryDescData->MeshRenderingData)
					{
						UINT TriangleNum = GeometryDescData->IndexSize / 3;
						// 遍历模型上的所有三角形
						for (UINT i = 0; i < TriangleNum; ++i)
						{
							XMINT3 Indices = {}; // xyz存储了三角形的索引
							Indices.x = GeometryDescData->MeshRenderingData->IndexData[GeometryDescData->IndexoffsetPosition + i * 3 + 0];
							Indices.y = GeometryDescData->MeshRenderingData->IndexData[GeometryDescData->IndexoffsetPosition + i * 3 + 1];
							Indices.z = GeometryDescData->MeshRenderingData->IndexData[GeometryDescData->IndexoffsetPosition + i * 3 + 2];

							XMFLOAT3 Vertex0 = GeometryDescData->MeshRenderingData->VertexData[Indices.x].Pos;
							XMFLOAT3 Vertex1 = GeometryDescData->MeshRenderingData->VertexData[Indices.y].Pos;
							XMFLOAT3 Vertex2 = GeometryDescData->MeshRenderingData->VertexData[Indices.z].Pos;
							XMVECTOR V0 = XMLoadFloat3(&Vertex0);
							XMVECTOR V1 = XMLoadFloat3(&Vertex1);
							XMVECTOR V2 = XMLoadFloat3(&Vertex2);

							float TriangleTestTime = 0.f;//碰撞到AABB中的三角形时间
							if (DirectX::TriangleTests::Intersects(LocalOriginPoint, LocalDirection, V0, V1, V2, TriangleTestTime))
							{
								//if(TriangleTestTime <= 0) continue;

								FinalTime = BoundTime;
								if (TriangleTestTime < TriangleTime)
								{
									TriangleTime = TriangleTestTime;

									OutHitResult.bHit = true;
									OutHitResult.Component_ = GeometryDescData->MeshComponet;
									OutHitResult.Time = TriangleTestTime;
									OutHitResult.GeometryDescData = GeometryDescData;
									//OutHitResult.Actor_ = static_pointer_cast<AActor>(GeometryDescData->MeshComponet->GetOuter()->shared_from_this());
									
									return true;
								}
							}
						}
					}
				}
			}
			
		}
	}
	//return true;
    return false;
}
