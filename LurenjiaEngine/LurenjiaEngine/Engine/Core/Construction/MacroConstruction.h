#pragma once
#include "MeshComponentConstruction.h"

#define INJECT_DATA_INTO_MESHCOMPONENT(T, ...)\
LurenjiaEngine::CreateMeshComponet<T>(GetMeshManage(), this, __VA_ARGS__);