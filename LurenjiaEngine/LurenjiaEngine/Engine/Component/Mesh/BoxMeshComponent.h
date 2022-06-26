#pragma once

#ifndef BoxMeshComponent_H
#define BoxMeshComponent_H

#include "StaticMeshComponent.h"

class CBoxMeshComponent : public CStaticMeshComponent
{
public:
	CBoxMeshComponent();

	void CreateMeshRenderData(FMeshRenderingData& InRenderingData, const float& Inheight, const float& Inwidth, const float& Indepth);
	void BuildKey(size_t& OutHashKey, const float& Inheight, const float& Inwidth, const float& Indepth);
};

#endif
