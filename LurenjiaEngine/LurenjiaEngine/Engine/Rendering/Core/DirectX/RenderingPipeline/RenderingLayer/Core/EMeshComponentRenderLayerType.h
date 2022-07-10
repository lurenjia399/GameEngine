#pragma once

#ifndef MeshComponentRenderLayerTypee_H
#define MeshComponentRenderLayerType_H

/// <summary>
/// 当前meshcomponent属于哪一个渲染层级
/// </summary>
enum class EMeshComponentRenderLayerType
{
	RENDERLAYER_OPAQUE,				//不透明
	RENDERLAYER_TRANSPARENT,		//透明
	RENDERLAYER_ALPHATEST,			//测试
	RENDERLAYER_BACKGROUND,			//背景
};

#endif // !MeshComponentType_H
