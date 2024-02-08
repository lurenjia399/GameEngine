#pragma once
#include "simple_library/public/simple_core_minimal/simple_c_core/simple_core_minimal.h"
#include "simple_library/public/simple_cpp_core_minimal/simple_cpp_core_minimal.h"
#include "simple_library/public/simple_library_macro.h"

namespace SimpleImage
{
	struct SIMPLE_LIBRARY_API FBmpImageInfo
	{
		BITMAPFILEHEADER BmpHearder;		//头信息
		BITMAPINFOHEADER BmpInFoHeader;		//头信息2
		RGBQUAD          BmpColors[256];	//颜色表
	};
}