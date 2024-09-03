#pragma once

#ifndef BOOLDETAILSMAPPING_H
#define BOOLDETAILSMAPPING_H

#include "../Core/PropertyDetailsMapping.h"

class FBoolDetailsMapping : public FPropertyDetailsMapping
{
public:

	void UpdateDetailsWidget(UProperty* InProperty) override;
public:
	static std::shared_ptr<FBoolDetailsMapping> MakeDetilsMapping();
};



#endif