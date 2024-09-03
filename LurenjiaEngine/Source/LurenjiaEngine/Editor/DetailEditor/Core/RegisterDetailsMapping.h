#pragma once

#ifndef REGISTERDETAILSMAPPING_H
#define REGISTERDETAILSMAPPING_H

#include "PropertyDetailsMapping.h"
#include "ClassDetailsMapping.h"

class FRegisterDetailsMapping 
{
	friend class FDetailsEditor;

public:
	static void RegisterClassDetails(const std::string& InClassName, shared_ptr<FClassDetailsMapping> InMapping);
	static void RegisterPropertyDetails(const std::string& InPropertyName, shared_ptr<FPropertyDetailsMapping> InMapping);
public:
	static std::map<std::string, shared_ptr<FClassDetailsMapping>> ClassDetailsMap;//
	static std::map<std::string, shared_ptr<FPropertyDetailsMapping>> PropertyDetailsMap;
};



#endif