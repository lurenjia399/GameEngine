#pragma once

#ifndef CylinderMeshComponent_H
#define CylinderMeshComponent_H

#include "StaticMeshComponent.h"

class CCylinderMeshComponent : public CStaticMeshComponent
{
public:
	CCylinderMeshComponent();

	void CreateMeshRenderData(FMeshRenderingData& InRenderingData, const float& InTopRadius, const float& InBottomRadius, const uint32_t& InAxialSubdivision, const float& InHeight, const uint32_t& InHeightSubdivision);
};

#endif
