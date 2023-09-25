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
	Width = 2048; 
	Height = 2048;
	GeometryMap = InGeometryMap;
	PSO = InPSO;
}
