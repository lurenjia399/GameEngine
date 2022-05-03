#pragma once

#ifndef ParallelLight
#define ParallelLight

#include "../Core/Actor.h"
#include "../../Component/TransformationComponent.h"


class AParallelLight : AActor
{
public:
	AParallelLight();
private:
	CVARIABLE()
	CTransformationComponent* TransformationComponent;
};


#endif