#pragma once
#include <vector>
#include <string>

namespace helper_tool_files
{
	

	// 将文件的每一行保存到数组中
	bool load_file_to_strings(const std::string& in_path, std::vector<std::string>& out_array);
	//bool save_file_to_strings(const std::string& in_path, const std::vector<std::string>& int_array);

	//bool load_file_to_bytes(const std::string& in_path, std::vector<unsigned char>& out_array);
	//bool save_file_to_bytes(const std::string& in_path, const std::vector<unsigned char>& int_array);

	unsigned int get_file_size_by_filename(const char* filename);
	unsigned int get_file_size(FILE* file_handle);

	bool get_file_buf(const char* path, char* buf);

	//从前往后找
	int find_string(const char* str, char const* sub_str, int start_pos);

	// 移除字符串中的子字符串
	void remove_string_start(char* str, char const* sub_str);

	// 去除前后的空格
	void trim_start_and_end_inline(char* buff);
	//去除前面的空格
	void trim_start_inline(char* buff);
	// 去除后边的空格
	void trim_end_inline(char* buff);

	// 从后边开始找模板串
	int find_string_from_end(const char* str, char const* sub_str, int start_pos);

	//拆分，将字符串按照划线，拆分成做优两部分,包含的划线在左半部分
	bool split(const char* buf, const char* str_split, char* l, char* r, bool bcontain_str_split);

	// 将字符串按照模板切割，并将两侧都存在数组中返回，不保存模板
	void parse_into_vector_array(const char* string_content, std::vector<std::string>& in_array, const char* parse_string);
}