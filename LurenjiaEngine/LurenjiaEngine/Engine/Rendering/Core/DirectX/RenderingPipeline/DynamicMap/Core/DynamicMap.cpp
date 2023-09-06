#include "DynamicMap.h"

FDynamicMap::FDynamicMap(int inWidth, int inHeight, FGeometryMap* inGeometryMap, FDirectXPiepelineState* inPSO)
	: Width(inWidth)
	, Height(inHeight)
	, GeometryMap(inGeometryMap)
	, PSO(inPSO)
{
}

void FDynamicMap::Init(FGeometryMap* InGeometryMap, FDirectXPiepelineState* InPSO)
{
	GeometryMap = InGeometryMap;
	PSO = InPSO;
}
