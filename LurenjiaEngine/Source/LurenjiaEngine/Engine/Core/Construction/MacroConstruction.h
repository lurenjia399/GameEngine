#pragma once
#include "MeshComponentConstruction.h"
#include "../../Component/TransformationComponent.h"

#define InjectComponentRenderDataByMesh(T,...)\
LurenjiaEngine::CreateMeshComponetRenderDataByMesh<T>(GetMeshManage(), this, __VA_ARGS__);

#define InjectComponentRenderDataByComponent(T,Component,...)\
LurenjiaEngine::CreateMeshComponetRenderDataByComponent<T>(GetMeshManage(), dynamic_cast<T*>(Component) , __VA_ARGS__);

//适合Actor构建
#define BUILD_OBJECT_PARAMETERS(Name,CoreActor)\
FCreateObjectParam Param##Name;\
Param##Name.Outer = CoreActor;\
Param##Name.ParentComponent = CoreActor->GetRootComponent();

//适合组件构建
#define BUILD_OBJECT_PARAMETERS_BY_COMPONENT(Name,CoreComponent)\
FCreateObjectParam Param##Name;\
Param##Name.Outer = CoreComponent;\
Param##Name.ParentComponent = CoreComponent;

//适合Object直接构建
#define BUILD_OBJECT_PARAMETERS_BY_NO_COMPONENT(Name,CoreComponent)\
FCreateObjectParam Param##Name;\
Param##Name.Outer = CoreComponent;
//##args