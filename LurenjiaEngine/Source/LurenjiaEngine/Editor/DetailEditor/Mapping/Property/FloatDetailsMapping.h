#pragma once

#ifndef FLOATDETAILSMAPPING_H
#define FLOATDETAILSMAPPING_H

#include "../../Core/PropertyDetailsMapping.h"

class FFloatDetailsMapping : public FPropertyDetailsMapping
{
public:

	void UpdateDetailsWidget(UProperty* InProperty) override;
public:
	static std::shared_ptr<FFloatDetailsMapping> MakeDetilsMapping();
};



#endif