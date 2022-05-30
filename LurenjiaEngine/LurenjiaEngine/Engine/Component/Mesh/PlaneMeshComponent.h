#pragma once

#ifndef PlaneMeshComponent_H
#define PlaneMeshComponent_H

#include "StaticMeshComponent.h"

class CPlaneMeshComponent : public CStaticMeshComponent
{
public:
	CPlaneMeshComponent();

	void CreateMeshRenderData(FMeshRenderingData& InRenderingData, const float& Inheight, const float& Inwidth, const uint32_t& InHeightSubdivide, const uint32_t& InwidthSubdivide);
};

#endif
