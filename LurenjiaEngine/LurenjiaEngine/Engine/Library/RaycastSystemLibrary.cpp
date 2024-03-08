#include "RaycastSystemLibrary.h"
#include "../Config/EngineRenderConfig.h"
#include "../Core/Camera.h"
#include "../Collision/CollisionSceneQuery.h"

bool FRaycastSystemLibrary::HitResultByScreen(shared_ptr<CWorld> InWorld, int ScreenX, int ScreenY, FHitResult& OutHitResult)
{
	if (shared_ptr<ACamera> camera = InWorld->GetCamera())
	{
		// 这个方法需要做的就是:
		// 从视口空间 变换到 摄像机空间
		// 视口空间 -> ndc空间 -> 透视乘法 -> 投影矩阵的逆 = 摄像机空间

		float z = 1.0f;//透视乘法的z轴，因为我们需要知道方向，所以不需要精确算出摄像机空间下对应鼠标点击的值。

		int width = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;
		int height = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;

		float ProjectX = camera->ProjectMatrix._11;
		float ProjectY = camera->ProjectMatrix._22;

		XMFLOAT2 view = XMFLOAT2(0.f, 0.f);// 这个就是在z为1的情况下，摄像机空间下的位置
		view.x = (2.0f * ScreenX / width - 1.0f) * z / ProjectX;
		view.y = (-2.0f * ScreenY / height + 1.0f) * z / ProjectY; // 注意这里符号，因为dx的视口原点在左上角，opengl的在左下角

		XMVECTOR ViewOriginPoint = XMVectorSet(0.f, 0.f, 0.f, 1.0f); // 摄像机空间下的远点，也就是摄像机所在的位置
		XMVECTOR ViewDirection = XMVectorSet(view.x, view.y, 1.0f, 0.f);// 射线方向
		//ViewDirection = XMVector3Normalize(ViewDirection);

		XMMATRIX World2ViewMatrix = XMLoadFloat4x4(&camera->ViewMatrix);
		XMVECTOR World2ViewMatrixDeterminant = DirectX::XMMatrixDeterminant(World2ViewMatrix);
		XMMATRIX World2ViewMatrixInverse = DirectX::XMMatrixInverse(&World2ViewMatrixDeterminant, World2ViewMatrix); // 求出摄像机矩阵的逆

		bool bHit = FCollisionSceneQuery::RaycastSingle(InWorld, ViewOriginPoint, ViewDirection, World2ViewMatrixInverse, OutHitResult);

		//if (!HitResult.Component_.expired())
		//{
		//	auto component = HitResult.Component_.lock();
		//	Engine_Log_Error("HitResult name[%s]", component->GetName().c_str());
		//}

		if (bHit)
		{
			return true;
		}
		
	}


    return false;
}
