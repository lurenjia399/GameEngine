#pragma once

#ifndef SphereMeshComponent_H
#define SphereMeshComponent_H

#include "StaticMeshComponent.h"

class CSphereMeshComponent : public CStaticMeshComponent
{
public:
	CSphereMeshComponent();

	void CreateMeshRenderData(FVertexRenderingData& InRenderingData, const float& InRadius, const uint32_t& InAxialSubdivision, const uint32_t& InHeightSubdivision);
	void BuildKey(size_t& OutHashKey, const float& InRadius, const uint32_t& InAxialSubdivision, const uint32_t& InHeightSubdivision); 
};

#endif
