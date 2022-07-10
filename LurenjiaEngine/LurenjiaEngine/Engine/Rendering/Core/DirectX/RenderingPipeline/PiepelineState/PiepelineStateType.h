#pragma once

#ifndef PiepelineState_H
#define PiepelineState_H

enum class EPiepelineStateType
{
	ALPHATEST = -1,		//测试没用
	TRANSPARENTS = 0,	//透明pso
	BACKGROUND = 1,		//背景pso
	WIREFRAME = 4,		//线框pso
	GRAYMODEL = 5,		//模型pso
};

#endif
