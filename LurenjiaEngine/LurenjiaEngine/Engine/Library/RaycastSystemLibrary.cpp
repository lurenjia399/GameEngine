#include "RaycastSystemLibrary.h"
#include "../Config/EngineRenderConfig.h"
#include "../Core/Camera.h"
#include "../Rendering/Core/DirectX/RenderingPipeline/Geometry/GeometryMap.h"

bool FRaycastSystemLibrary::HitResultByScreen(CWorld* InWorld, int ScreenX, int ScreenY, FHitResult& OutHitResult)
{
	if (ACamera* camera = InWorld->GetCamera())
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
		view.y = (2.0f * ScreenY / height - 1.0f) * z / ProjectY;

		XMVECTOR OriginPoint = XMVectorSet(0.f, 0.f, 0.f, 1.0f); // 摄像机空间下的远点，也就是摄像机所在的位置
		XMVECTOR Direction = XMVectorSet(view.x, view.y, 1.f, 0.f);// 射线方向


		for (std::weak_ptr<FGeometryDescData> data : FGeometry::MeshRenderingDataPool)
		{

		}
	}
	
    //InWorld->LineTraceSingleByChannel(OutHitResult);

    return false;
}
