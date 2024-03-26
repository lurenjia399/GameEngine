#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include "../../../../simple_library/public/simple_cpp_core_minimal/simple_cpp_helper_file/simple_cpp_helper_file.h"
#include "../../../../simple_library/public/simple_core_minimal/simple_c_helper_file/simple_file_helper.h"
#include "../../../../simple_library/public/simple_core_minimal/simple_c_path/simple_path.h"
#include "../../../../simple_library/public/simple_cpp_core_minimal/simple_cpp_string_algorithm/simple_cpp_string_algorithm.h"
#include <string.h>

namespace simple_cpp_helper_file
{
	bool simple_cpp_helper_file::load_file_to_strings(
		const std::string& in_path,
		std::vector<std::string>& out_array)
	{
		//get files size
		int file_size = get_file_size_by_filename(in_path.c_str());

		if (file_size != 0)
		{
			//malloc space
			char* file_buff = (char*)malloc(file_size + 1);
			memset(file_buff, 0, file_size + 1);

			//read files
			if (get_file_buf(in_path.c_str(), file_buff))
			{
				file_buff[file_size] = '\0';

				
				const char end_str_carriage_return = '\n';
				const char end_str_table = '\t';

				
				char end_str_line_offset = 0;

				
				int start_offset = 0;
				int end_offset = 0;

				auto cpy_string = [&](int i)->bool
				{
					
					start_offset = end_offset;
					end_offset = i;

					
					int len = end_offset - start_offset;

					
					out_array.push_back(std::string());
					std::string& pre_line_buff_string = out_array[out_array.size() - 1];
					if (len > 0)
					{
						
						pre_line_buff_string.resize(len);

						
						char* pre_line = const_cast<char*>(pre_line_buff_string.c_str());

						
						len -= end_str_line_offset;

						
						char* content = strncpy(
							pre_line,
							&file_buff[start_offset + end_str_line_offset],
							len);

						
						end_str_line_offset = 1;
						return true;
					}

					return false;
				};

				
				for (int i = 0; i < file_size + 1; i++)
				{
					if (file_buff[i] == end_str_carriage_return)
					{
						
						if ((i + 1) < (file_size + 1))
						{
							if (cpy_string(i))
							{

							}
						}
					}

					
					if (i == file_size)
					{
						if (cpy_string(i))
						{

						}
					}
				}
			}

			free(file_buff);
			file_buff = nullptr;
		}

		return out_array.size() > 0;
	}

	bool SIMPLE_LIBRARY_API save_file_to_strings(const std::string& in_path, const std::vector<std::string>& in_array)
	{
		
		if (!is_file_exists(in_path.c_str()))
		{
			
			char path_directory[1024] = { 0 };
			get_path_directory(path_directory, in_path.c_str());

			
			if (create_file_directory(path_directory))
			{
				
				create_file(in_path.c_str());
			}
		}

		
		int final_num = 0;
		for (auto& tmp : in_array)
		{
			final_num += (int)strlen(tmp.c_str());
		}

		
		final_num += (int)in_array.size();

		
		std::string final_str;
		
		final_str.resize(final_num + 1);

		
		int start_pos = 0;
		for (auto& tmp : in_array)
		{
			int tmp_str_len = (int)strlen(tmp.c_str());
			if (simple_cpp_string_algorithm::index_valid((int)final_str.size(), start_pos))
			{
				char* ptr = &final_str[start_pos];
				strcpy(ptr, tmp.c_str());

				int offset = 0;
				if (ptr[tmp_str_len - 1] == '\0')
				{
					int index = 1;
					//check
					while (ptr[tmp_str_len - index] == '\0')
					{
						index++;
					}

					for (int i = index; i > 0; i--)
					{
						ptr[tmp_str_len - i] = ' ';
					}

					ptr[tmp_str_len - index] = '\n';
				}
				else
				{
					offset++;
					ptr[tmp_str_len] = '\n';
				}

				start_pos += (tmp_str_len + offset);
			}
		}

		final_str[final_num] = '\0';

		char* buff = const_cast<char*>(final_str.c_str());

		return save_file_buff(in_path.c_str(), buff);;
	}

	bool load_file_to_bytes(const std::string& in_path, std::vector<unsigned char>& out_array)
	{
		int file_size = get_file_size_by_filename(in_path.c_str());

		if (file_size != 0)
		{
			out_array.resize(file_size);

			return load_data_from_disk(in_path.c_str(), (char*)out_array.data());
		}

		return false;
	}

	bool save_file_to_bytes(const std::string& in_path, const std::vector<unsigned char>& int_array)
	{
		return save_data_to_disk(in_path.c_str(), (char*)int_array.data(), (int)int_array.size());
	}
}

#endif