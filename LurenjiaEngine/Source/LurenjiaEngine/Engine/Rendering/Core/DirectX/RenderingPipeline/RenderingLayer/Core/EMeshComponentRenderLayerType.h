#pragma once

#ifndef MeshComponentRenderLayerTypee_H
#define MeshComponentRenderLayerType_H

/// <summary>
/// ��ǰmeshcomponent������һ����Ⱦ�㼶
/// </summary>
enum class EMeshComponentRenderLayerType
{
	RENDERLAYER_OPERATIONHANDLE,	//������ͷ		priority = 3200
	RENDERLAYER_SELECT,				//ѡ��			priority = 3100
	RENDERLAYER_TRANSPARENT,		//͸��			priority = 3000
	RENDERLAYER_ALPHATEST,			//����			priority = 2450
	RENDERLAYER_OPAQUEREFLECT,		//��͸������		priority = 2100
	RENDERLAYER_OPAQUE,				//��͸��			priority = 2000
	RENDERLAYER_OPAQUESHADOW,		//��͸����Ӱ		priority = 1280
	RENDERLAYER_BACKGROUND,			//����			priority = 100
	
	
};

#endif // !MeshComponentType_H
