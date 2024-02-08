#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif // !_CRT_SECURE_NO_WARNINGS

#include "simple_library/public/simple_cpp_core_minimal/simple_cpp_helper_file/simple_cpp_helper_file.h"
#include "simple_library/public/simple_core_minimal/simple_c_helper_file/simple_file_helper.h"
#include "simple_library/public/simple_core_minimal/simple_c_path/simple_path.h"
#include "simple_library/public/simple_cpp_core_minimal/simple_cpp_string_algorithm/simple_cpp_string_algorithm.h"
#include <string.h>

namespace simple_cpp_helper_file
{
	bool simple_cpp_helper_file::load_file_to_strings(
		const std::string& in_path,
		std::vector<std::string>& out_array)
	{
		//获取文件大小
		int file_size = get_file_size_by_filename(in_path.c_str());

		if (file_size != 0)
		{
			//分配大小
			char* file_buff = (char*)malloc(file_size + 1);
			memset(file_buff, 0, file_size + 1);

			//从磁盘读取文件
			if (get_file_buf(in_path.c_str(), file_buff))
			{
				file_buff[file_size] = '\0';//最后一位用 /0结束

				//提取每行的数据 没行数据以 \n结尾
				const char end_str_carriage_return = '\n';
				const char end_str_table = '\t';

				//基于 \n的偏移
				char end_str_line_offset = 0;

				//记录开始
				int start_offset = 0;
				int end_offset = 0;

				auto cpy_string = [&](int i)->bool
				{
					//记录上次的开始
					start_offset = end_offset;
					end_offset = i;

					//拿到长度
					int len = end_offset - start_offset;

					//添加一个字符串
					out_array.push_back(std::string());
					std::string& pre_line_buff_string = out_array[out_array.size() - 1];
					if (len > 0)
					{
						//先划分缓冲区
						pre_line_buff_string.resize(len);

						//拿到每条string数据
						char* pre_line = const_cast<char*>(pre_line_buff_string.c_str());

						//减去/n的那个
						len -= end_str_line_offset;

						//开始拷贝
						char* content = strncpy(
							pre_line,
							&file_buff[start_offset + end_str_line_offset],
							len);

						//跳过 /n
						end_str_line_offset = 1;
						return true;
					}

					return false;
				};

				//一行行的切割字符串
				for (int i = 0; i < file_size + 1; i++)
				{
					if (file_buff[i] == end_str_carriage_return)
					{
						//数量是不是已经大于文件 也就是判定是不是最后一个
						if ((i + 1) < (file_size + 1))
						{
							if (cpy_string(i))
							{

							}
						}
					}

					//处理最后一个字符串
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
		//文件是否存在
		if (!is_file_exists(in_path.c_str()))
		{
			//准备好路径
			char path_directory[1024] = { 0 };
			get_path_directory(path_directory, in_path.c_str());

			//先创建路径
			if (create_file_directory(path_directory))
			{
				//创建对应的文件
				create_file(in_path.c_str());
			}
		}

		//获取一共多少
		int final_num = 0;
		for (auto& tmp : in_array)
		{
			final_num += strlen(tmp.c_str());
		}

		//为/n留下空间
		final_num += in_array.size();

		//先划分一个缓冲区
		std::string final_str;
		//+1 是\0结尾
		final_str.resize(final_num + 1);

		//判定按照什么方式结尾
		int start_pos = 0;
		for (auto& tmp : in_array)
		{
			int tmp_str_len = strlen(tmp.c_str());
			if (simple_cpp_string_algorithm::index_valid(final_str.size(), start_pos))
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
		return save_data_to_disk(in_path.c_str(), (char*)int_array.data(), int_array.size());
	}
}