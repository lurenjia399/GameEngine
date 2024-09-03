#pragma once

#ifndef CLASSDETAILSMAPPING_H
#define CLASSDETAILSMAPPING_H


#include "DetailsMapping.h"

class FClassDetailsMapping : public FDetailsMapping
{
public:
	static std::shared_ptr<FClassDetailsMapping> MakeDetailsMapping() { return nullptr; }

public:
};



#endif