#pragma once
//Copyright (C) RenZhai.2019.All Rights Reserved.
#include "../../../../simple_library/public/simple_core_minimal/simple_c_core/simple_core_minimal.h"

_CRT_BEGIN_C_HEADER
typedef enum 
{
	SIMPLE_BLACK = 0,				// ��ɫ		
	SIMPLE_BLUE,					// ��ɫ		
	SIMPLE_GREEN,					// ��ɫ		
	SIMPLE_LIGHT_BLUE,				// ǳ��ɫ	
	SIMPLE_RED,						// ��ɫ		
	SIMPLE_VIOLET,					// ��ɫ		
	SIMPLE_YELLOW,					// ��ɫ		
	SIMPLE_WHITE,					// ��ɫ		
	SIMPLE_GREY,					// ��ɫ
	SIMPLE_PALE_BLUE,				// ����ɫ
	SIMPLE_PALE_GREEN,				// ����ɫ
	SIMPLE_LIGHT_GREEN,				// ��ǳ��ɫ
	SIMPLE_PALE_RED,				// ����ɫ
	SIMPLE_LAVENDER,				// ����ɫ
	SIMPLE_CANARY_YELLOW,			// ����ɫ
	SIMPLE_BRIGHT_WHITE,			// ����ɫ
}simple_console_w_color;

//set_console_windows_color
void set_console_w_color(simple_console_w_color font_color, simple_console_w_color background_color);
_CRT_END_C_HEADER