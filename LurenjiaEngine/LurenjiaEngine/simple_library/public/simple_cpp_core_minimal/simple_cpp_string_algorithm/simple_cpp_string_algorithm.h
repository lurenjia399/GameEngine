// Copyright (C) RenZhai.2022.All Rights Reserved.
#pragma once
#include <vector>
#include <string>
#include "simple_library/public/simple_library_macro.h"

#define STRING_SIZE_PRINTF 2048

namespace simple_cpp_string_algorithm
{
	bool SIMPLE_LIBRARY_API string_contain(const std::string& buff,const char* find_buff);
	void SIMPLE_LIBRARY_API parse_into_vector_array(const char *string_content,std::vector<std::string> &in_array, const char *parse_string);

	bool SIMPLE_LIBRARY_API index_valid(int in_size, int index);

	std::string SIMPLE_LIBRARY_API printf(const char* format, ...);
}