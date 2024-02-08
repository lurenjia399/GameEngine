#pragma once
#include "simple_library/public/simple_cpp_core_minimal/simple_cpp_core_minimal.h"
#include "simple_library/public/simple_image/Core/simple_image_type.h"
#include "simple_library/public/simple_library_macro.h"

namespace SimpleImage
{
	bool SIMPLE_LIBRARY_API GetRGBImageDataExcludeHeaderInfor(int InWidth,int InHeight, std::vector<unsigned char>& InData);
	bool SIMPLE_LIBRARY_API GetImageDataExcludeHeaderInfor(const FImageInfo& InImageInfo,std::vector<unsigned char> &InData);

	bool SIMPLE_LIBRARY_API SaveImageToDisk(const FImageInfo &InImageInfo, const std::wstring& InSavePath, const std::vector<unsigned char> &InData);
	bool SIMPLE_LIBRARY_API LoadImageToDisk(EImageType ImageType, const std::wstring& InLoadPath, std::vector<unsigned char> &OutData,FImageInfo *OutInfo = NULL);
}