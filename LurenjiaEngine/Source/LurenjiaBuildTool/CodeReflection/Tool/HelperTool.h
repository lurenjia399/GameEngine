#pragma once
#include <vector>
#include <string>
#include <io.h>
#include <direct.h>
#include <stdarg.h> 

#define STRING_SIZE_PRINTF 2048
namespace helper_tool_files
{
	// 替换文件中的\为/
	void normalization_path(char* path_buf);

	// 是否包含模板串
	bool string_contain(const std::string& buff, const char* find_buff);
	// 从前往后找模板串
	int find_string(const char* str, char const* sub_str, int start_pos);
	// 从后往前找模板串
	int find_string_from_end(const char* str, char const* sub_str, int start_pos);


	// 获取文件大小通过文件名称
	unsigned int get_file_size_by_filename(const char* filename);
	// 获取文件大小通过文件句柄
	unsigned int get_file_size(FILE* file_handle);
	// 获取文件字符指针
	bool get_file_buf(const char* path, char* buf);
	// 获取文件的每一行保存到数组中
	bool load_file_to_strings(const std::string& in_path, std::vector<std::string>& out_array);
	//bool save_file_to_strings(const std::string& in_path, const std::vector<std::string>& int_array);
	//bool load_file_to_bytes(const std::string& in_path, std::vector<unsigned char>& out_array);
	//bool save_file_to_bytes(const std::string& in_path, const std::vector<unsigned char>& int_array);

	// 保存 将字符串保存到文件中(不好使，还没写)
	bool save_file_to_strings(const std::string& in_path, const std::vector<std::string>& in_array);
	// 文件是否存在(不好使，还没写)
	bool is_file_exists(char const* filename);
	// 创建文件夹(不好使，还没写)
	bool create_file_directory(char const* in_path);
	// 获取据
	void get_path_directory(char* buf, int length, const char* path_buf);



	// 移除字符串中的子字符串
	void remove_string_start(char* str, char const* sub_str);
	// 移除字符,从前往后遍历
	bool remove_char_start(char* str, char sub_str);
	// 移除字符,从后往前遍历
	bool remove_char_end(char* str, char sub_str);
	// 去除前后的空格
	void trim_start_and_end_inline(char* buff);
	// 去除前面的空格
	void trim_start_inline(char* buff);
	// 去除后边的空格
	void trim_end_inline(char* buff);



	// 拆分，以正向寻找str_split并作为轴，拆分成左右两部分,轴会包含在左半部分
	bool split(const char* buf, const char* str_split, char* l, char* r, bool bcontain_str_split);
	// 拆分，以反向寻找str_split并作为轴，拆分成左右两部分,轴会包含在左半部分
	bool split_end(const char* buf, const char* str_split, char* l, char* r, bool bcontain_str_split);
	// 拆分，将字符串按照模板切割，并将两侧都存在数组中返回，不保存模板
	void parse_into_vector_array(const char* string_content, std::vector<std::string>& in_array, const char* parse_string);

	// 输出字符串
	std::string printf(const char* format, ...);
}