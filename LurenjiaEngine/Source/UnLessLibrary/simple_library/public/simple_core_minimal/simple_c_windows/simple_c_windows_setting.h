#pragma once
//Copyright (C) RenZhai.2019.All Rights Reserved.
#include "../../../../simple_library/public/simple_core_minimal/simple_c_core/simple_core_minimal.h"

_CRT_BEGIN_C_HEADER
typedef enum 
{
	SIMPLE_BLACK = 0,				// 黑色		
	SIMPLE_BLUE,					// 蓝色		
	SIMPLE_GREEN,					// 绿色		
	SIMPLE_LIGHT_BLUE,				// 浅绿色	
	SIMPLE_RED,						// 红色		
	SIMPLE_VIOLET,					// 紫色		
	SIMPLE_YELLOW,					// 黄色		
	SIMPLE_WHITE,					// 白色		
	SIMPLE_GREY,					// 灰色
	SIMPLE_PALE_BLUE,				// 淡蓝色
	SIMPLE_PALE_GREEN,				// 淡绿色
	SIMPLE_LIGHT_GREEN,				// 淡浅绿色
	SIMPLE_PALE_RED,				// 淡红色
	SIMPLE_LAVENDER,				// 淡紫色
	SIMPLE_CANARY_YELLOW,			// 淡黄色
	SIMPLE_BRIGHT_WHITE,			// 亮白色
}simple_console_w_color;

//set_console_windows_color
void set_console_w_color(simple_console_w_color font_color, simple_console_w_color background_color);
_CRT_END_C_HEADER