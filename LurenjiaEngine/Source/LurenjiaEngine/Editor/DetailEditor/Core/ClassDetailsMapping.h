#pragma once

#ifndef CLASSDETAILSMAPPING_H
#define CLASSDETAILSMAPPING_H


#include "DetailsMapping.h"
#include "CoreObject/CoreMinimalObject.h"

class FClassDetailsMapping : public FDetailsMapping
{
public:

	virtual void UpdateDetailsWidget(CCoreMinimalObject* InCoreMinimalObject);

	static std::shared_ptr<FClassDetailsMapping> MakeDetailsMapping() { return nullptr; }

};



#endif