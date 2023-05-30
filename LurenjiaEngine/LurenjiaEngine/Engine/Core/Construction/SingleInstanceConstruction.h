#pragma once

#ifndef SingleInstanceConstruction_H
#define SingleInstanceConstruction_H

//#include "MacroConstruction.h"

class CCoreMinimalObject;

namespace LurenjiaEngine
{
	template<typename T>
	class CreateSingleInstance
	{
	public:
		CreateSingleInstance()
		{
			if (instance == nullptr)
			{
				instance = new T();
			}
		}
		static T* Getinstance()
		{
			return instance;
		}
	private:
		static T* instance;
	};
	
	template<typename T>
	T* CreateSingleInstance<T>::instance = nullptr;
}

#endif

