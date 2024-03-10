#include "CollisionSceneQuery.h"
#include "../Rendering/Core/DirectX/RenderingPipeline/Geometry/GeometryMap.h"

bool FCollisionSceneQuery::RaycastSingle(shared_ptr<CWorld> InWorld, const XMVECTOR& ViewOriginPoint, const XMVECTOR& ViewDirection, const XMMATRIX& World2ViewMatrixInverse, FHitResult& OutHitResult)
{
	float FinalTime = FLT_MAX;
	// ���������е�����ģ��
	for (std::weak_ptr<FGeometryDescData> data_weak : FGeometry::MeshRenderingDataPool)
	{
		auto GeometryDescData = data_weak.lock();

		XMMATRIX Local2WorldMatrix = XMLoadFloat4x4(&GeometryDescData->WorldMatrix);// ���������������е�
		XMVECTOR Local2WorldMatrixDeterminant = DirectX::XMMatrixDeterminant(Local2WorldMatrix);
		XMMATRIX Local2WorldMatrixInverse = DirectX::XMMatrixInverse(&Local2WorldMatrixDeterminant, Local2WorldMatrix); // ��������������

		
		// ע�����˳�����˳���ô�� ������������ * ���������� ,,,�洢�ľ�������һ���ģ����Ǵ����������У�shader������
		XMMATRIX View2LocalMatrix = XMMatrixMultiply(World2ViewMatrixInverse, Local2WorldMatrixInverse);

		XMVECTOR LocalOriginPoint = DirectX::XMVector3TransformCoord(ViewOriginPoint, View2LocalMatrix);
		XMVECTOR LocalDirection = DirectX::XMVector3TransformNormal(ViewDirection, View2LocalMatrix);
		LocalDirection = DirectX::XMVector3Normalize(LocalDirection);

		float BoundTime = 0.f; //��ײ��AABB��ʱ��
		float TriangleTime = FLT_MAX;
		// �ҵ���ģ��AABB�н����ģ��
		if (GeometryDescData->AABB_box.Intersects(LocalOriginPoint, LocalDirection, BoundTime))
		{
			if (BoundTime > 0)
			{
				if (BoundTime < FinalTime)
				{
					if (GeometryDescData->MeshRenderingData)
					{
						UINT TriangleNum = GeometryDescData->IndexSize / 3;
						// ����ģ���ϵ�����������
						for (UINT i = 0; i < TriangleNum; ++i)
						{
							XMINT3 Indices = {}; // xyz�洢�������ε�����
							Indices.x = GeometryDescData->MeshRenderingData->IndexData[GeometryDescData->IndexoffsetPosition + i * 3 + 0];
							Indices.y = GeometryDescData->MeshRenderingData->IndexData[GeometryDescData->IndexoffsetPosition + i * 3 + 1];
							Indices.z = GeometryDescData->MeshRenderingData->IndexData[GeometryDescData->IndexoffsetPosition + i * 3 + 2];

							XMFLOAT3 Vertex0 = GeometryDescData->MeshRenderingData->VertexData[Indices.x].Pos;
							XMFLOAT3 Vertex1 = GeometryDescData->MeshRenderingData->VertexData[Indices.y].Pos;
							XMFLOAT3 Vertex2 = GeometryDescData->MeshRenderingData->VertexData[Indices.z].Pos;
							XMVECTOR V0 = XMLoadFloat3(&Vertex0);
							XMVECTOR V1 = XMLoadFloat3(&Vertex1);
							XMVECTOR V2 = XMLoadFloat3(&Vertex2);

							float TriangleTestTime = 0.f;//��ײ��AABB�е�������ʱ��
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
