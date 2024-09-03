#pragma once

#ifndef PROPERTYDETAILSMAPPING_H
#define PROPERTYDETAILSMAPPING_H

#include "DetailsMapping.h"

class UProperty;

class FPropertyDetailsMapping : public FDetailsMapping
{
public:

	virtual void UpdateDetailsWidget(UProperty* InProperty);
public:
	static std::shared_ptr<FPropertyDetailsMapping> MakeDetilsMapping() { return nullptr; }
};



#endif