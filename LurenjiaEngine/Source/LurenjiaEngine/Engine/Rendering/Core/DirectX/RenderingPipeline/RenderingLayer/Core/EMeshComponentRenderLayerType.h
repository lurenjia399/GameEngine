#pragma once

#ifndef MeshComponentRenderLayerTypee_H
#define MeshComponentRenderLayerType_H

/// <summary>
/// 当前meshcomponent属于哪一个渲染层级
/// </summary>
enum class EMeshComponentRenderLayerType
{
	RENDERLAYER_OPERATIONHANDLE,	//操作箭头		priority = 3200
	RENDERLAYER_SELECT,				//选择			priority = 3100
	RENDERLAYER_TRANSPARENT,		//透明			priority = 3000
	RENDERLAYER_ALPHATEST,			//测试			priority = 2450
	RENDERLAYER_OPAQUEREFLECT,		//不透明反射		priority = 2100
	RENDERLAYER_OPAQUE,				//不透明			priority = 2000
	RENDERLAYER_OPAQUESHADOW,		//不透明阴影		priority = 1280
	RENDERLAYER_BACKGROUND,			//背景			priority = 100
	
	
};

#endif // !MeshComponentType_H
