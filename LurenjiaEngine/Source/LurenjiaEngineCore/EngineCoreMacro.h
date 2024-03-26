#pragma once

#ifndef LURENJIAENGINE_API_EXPORTS
#define LURENJIAENGINE_API_EXPORTS
#endif

#ifdef LURENJIAENGINE_API_EXPORTS
#define LURENJIAENGINE_API __declspec(dllexport)
#else
#define LURENJIAENGINE_API __declspec(dllimport)
#endif 