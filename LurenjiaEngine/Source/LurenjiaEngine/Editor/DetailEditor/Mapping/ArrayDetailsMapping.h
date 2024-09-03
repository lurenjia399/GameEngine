#pragma once

#ifndef ARRAYDETAILSMAPPING_H
#define ARRAYDETAILSMAPPING_H

#include "../Core/PropertyDetailsMapping.h"

class FArrayDetailsMapping : public FPropertyDetailsMapping
{
public:

	void UpdateDetailsWidget(UProperty* InProperty) override;
public:
	static std::shared_ptr<FArrayDetailsMapping> MakeDetilsMapping();
};



#endif