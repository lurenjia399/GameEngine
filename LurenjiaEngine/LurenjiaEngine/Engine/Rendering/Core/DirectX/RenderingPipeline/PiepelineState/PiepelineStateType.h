#pragma once

#ifndef PiepelineState_H
#define PiepelineState_H

enum class EPiepelineStateType
{
	ALPHATEST = -1,		//����û��
	TRANSPARENTS = 0,	//͸��pso
	BACKGROUND = 1,		//����pso
	WIREFRAME = 4,		//�߿�pso
	GRAYMODEL = 5,		//ģ��pso
};

#endif
