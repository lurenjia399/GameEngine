#pragma once
#include "MeshComponentConstruction.h"

#define InjectComponentRenderDataByMesh(T, ...)\
LurenjiaEngine::CreateMeshComponetRenderDataByMesh<T>(GetMeshManage(), this, __VA_ARGS__);

#define InjectComponentRenderDataByComponent(T, Component, ...)\
LurenjiaEngine::CreateMeshComponetRenderDataByComponent<T>(GetMeshManage(), Component , __VA_ARGS__);