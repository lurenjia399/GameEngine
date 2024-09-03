#pragma once

#ifndef STRINGDETAILSMAPPING_H
#define STRINGDETAILSMAPPING_H

#include "../Core/PropertyDetailsMapping.h"

class FStringDetailsMapping : public FPropertyDetailsMapping
{
public:
	void UpdateDetailsWidget(UProperty* InProperty) override;

	static int callback(ImGuiInputTextCallbackData* InData);
public:
	static std::shared_ptr<FStringDetailsMapping> MakeDetilsMapping();
};



#endif