#pragma once

#ifndef INTDETAILSMAPPING_H
#define INTDETAILSMAPPING_H

#include "../../Core/PropertyDetailsMapping.h"

class FIntDetailsMapping : public FPropertyDetailsMapping
{
public:

	void UpdateDetailsWidget(UProperty* InProperty) override;
public:
	static std::shared_ptr<FIntDetailsMapping> MakeDetilsMapping();
};



#endif