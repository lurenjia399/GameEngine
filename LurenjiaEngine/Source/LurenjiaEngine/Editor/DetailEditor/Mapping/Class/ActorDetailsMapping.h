#pragma once

#ifndef ACTORDETAILSMAPPING_H
#define ACTORDETAILSMAPPING_H

#include "../../Core/ClassDetailsMapping.h"

class FActorDetailsMapping : public FClassDetailsMapping
{
public:
	void UpdateDetailsWidget(CCoreMinimalObject* InCoreMinimalObject) override;

public:
	static std::shared_ptr<FActorDetailsMapping> MakeDetilsMapping();
};



#endif