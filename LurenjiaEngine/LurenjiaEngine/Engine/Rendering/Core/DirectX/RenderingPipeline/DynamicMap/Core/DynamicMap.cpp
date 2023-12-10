#include "DynamicMap.h"
#include "../../../../../../Config/EngineRenderConfig.h"

FDynamicMap::FDynamicMap(int inWidth, int inHeight, FGeometryMap* inGeometryMap, FDirectXPiepelineState* inPSO)
	: Width(inWidth)
	, Height(inHeight)
	, GeometryMap(inGeometryMap)
	, PSO(inPSO)
{
}

void FDynamicMap::Init(FGeometryMap* InGeometryMap, FDirectXPiepelineState* InPSO)
{
	Width = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;
	Height = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;
	GeometryMap = InGeometryMap;
	PSO = InPSO;
}
