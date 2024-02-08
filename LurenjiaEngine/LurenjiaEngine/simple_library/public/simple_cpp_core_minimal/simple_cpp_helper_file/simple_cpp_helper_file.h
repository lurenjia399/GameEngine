// Copyright (C) RenZhai.2022.All Rights Reserved.
#pragma once
#include <vector>
#include <string>
#include "simple_library/public/simple_library_macro.h"

namespace simple_cpp_helper_file
{
	bool SIMPLE_LIBRARY_API load_file_to_strings(const std::string& in_path, std::vector<std::string>& out_array);
	bool SIMPLE_LIBRARY_API save_file_to_strings(const std::string& in_path, const std::vector<std::string>& int_array);

	bool SIMPLE_LIBRARY_API load_file_to_bytes(const std::string& in_path, std::vector<unsigned char>& out_array);
	bool SIMPLE_LIBRARY_API save_file_to_bytes(const std::string& in_path, const std::vector<unsigned char>& int_array);
}