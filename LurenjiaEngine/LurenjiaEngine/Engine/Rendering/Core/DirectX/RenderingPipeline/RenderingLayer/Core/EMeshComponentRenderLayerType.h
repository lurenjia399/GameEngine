#pragma once

#ifndef MeshComponentRenderLayerTypee_H
#define MeshComponentRenderLayerType_H

/// <summary>
/// ��ǰmeshcomponent������һ����Ⱦ�㼶
/// </summary>
enum class EMeshComponentRenderLayerType
{
	RENDERLAYER_OPAQUE,				//��͸��
	RENDERLAYER_TRANSPARENT,		//͸��
	RENDERLAYER_ALPHATEST,			//����
	RENDERLAYER_BACKGROUND,			//����
	RENDERLAYER_OPAQUEREFLECT,		//��͸������
};

#endif // !MeshComponentType_H
