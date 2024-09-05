#pragma once

#ifndef VECTOR3DETAILSMAPPING_H
#define VECTOR3DETAILSMAPPING_H

#include "../../Core/PropertyDetailsMapping.h"

class FVector3DetailsMapping : public FPropertyDetailsMapping
{
public:

	void UpdateDetailsWidget(UProperty* InProperty) override;
public:
	static std::shared_ptr<FVector3DetailsMapping> MakeDetilsMapping();
};



#endif