#include "CollisionSceneQuery.h"
#include "../Rendering/Core/DirectX/RenderingPipeline/Geometry/GeometryMap.h"

bool FCollisionSceneQuery::RaycastSingle(CWorld* InWorld, const XMVECTOR& ViewOriginPoint, const XMVECTOR& ViewDirection, const XMMATRIX& World2ViewMatrixInverse, FHitResult& HitResult)
{
	for (std::weak_ptr<FGeometryDescData> data_weak : FGeometry::MeshRenderingDataPool)
	{
		auto GeometryDescData = data_weak.lock();

		XMMATRIX Local2WorldMatrix = XMLoadFloat4x4(&GeometryDescData->WorldMatrix);// ���������������е�
		XMVECTOR Local2WorldMatrixDeterminant = DirectX::XMMatrixDeterminant(Local2WorldMatrix);
		XMMATRIX Local2WorldMatrixInverse = DirectX::XMMatrixInverse(&Local2WorldMatrixDeterminant, Local2WorldMatrix); // ��������������

		// ע�����˳�����˳���ô�� ������������ * ���������� ,,,�洢�ľ�������һ���ģ����Ǵ����������У�shader������
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
