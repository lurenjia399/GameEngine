#pragma once

#ifndef MAPDETAILSMAPPING_H
#define MAPDETAILSMAPPING_H

#include "../Core/PropertyDetailsMapping.h"

class FMapDetailsMapping : public FPropertyDetailsMapping
{
public:

	void UpdateDetailsWidget(UProperty* InProperty) override;
public:
	static std::shared_ptr<FMapDetailsMapping> MakeDetilsMapping();
};



#endif