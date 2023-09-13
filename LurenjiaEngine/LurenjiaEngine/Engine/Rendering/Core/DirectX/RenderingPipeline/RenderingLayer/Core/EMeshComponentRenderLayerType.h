#pragma once

#ifndef MeshComponentRenderLayerTypee_H
#define MeshComponentRenderLayerType_H

/// <summary>
/// ��ǰmeshcomponent������һ����Ⱦ�㼶
/// </summary>
enum class EMeshComponentRenderLayerType
{
	RENDERLAYER_OPAQUE,				//��͸�� priority = 2000
	RENDERLAYER_TRANSPARENT,		//͸�� priority = 30000
	RENDERLAYER_ALPHATEST,			//���� priority = 2450
	RENDERLAYER_BACKGROUND,			//���� priority = 100
	RENDERLAYER_OPAQUEREFLECT,		//��͸������ priority = 2100
	RENDERLAYER_OPAQUESHADOW,		//��͸����Ӱ priority = 1280
};

#endif // !MeshComponentType_H
