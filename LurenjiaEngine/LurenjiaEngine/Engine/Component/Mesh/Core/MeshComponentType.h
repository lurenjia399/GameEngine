#pragma once

#ifndef MeshComponentType_H
#define MeshComponentType_H

/// <summary>
/// 当前meshcomponent属于哪一个渲染层级
/// </summary>
enum class EMeshComponentRenderingLayerType
{
	RENDERLAYER_OPAQUE,				//不透明
	RENDERLAYER_TRANSPARENT,		//透明
	RENDERLAYER_ALPHATEST,			//测试
};

#endif // !MeshComponentType_H
