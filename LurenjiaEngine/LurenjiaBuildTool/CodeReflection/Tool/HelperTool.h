#pragma once
#include <vector>
#include <string>

namespace helper_tool_files
{
	

	// ���ļ���ÿһ�б��浽������
	bool load_file_to_strings(const std::string& in_path, std::vector<std::string>& out_array);
	//bool save_file_to_strings(const std::string& in_path, const std::vector<std::string>& int_array);

	//bool load_file_to_bytes(const std::string& in_path, std::vector<unsigned char>& out_array);
	//bool save_file_to_bytes(const std::string& in_path, const std::vector<unsigned char>& int_array);

	unsigned int get_file_size_by_filename(const char* filename);
	unsigned int get_file_size(FILE* file_handle);

	bool get_file_buf(const char* path, char* buf);

	//��ǰ������
	int find_string(const char* str, char const* sub_str, int start_pos);

	// �Ƴ��ַ����е����ַ���
	void remove_string_start(char* str, char const* sub_str);

	// ȥ��ǰ��Ŀո�
	void trim_start_and_end_inline(char* buff);
	//ȥ��ǰ��Ŀո�
	void trim_start_inline(char* buff);
	// ȥ����ߵĿո�
	void trim_end_inline(char* buff);

	// �Ӻ�߿�ʼ��ģ�崮
	int find_string_from_end(const char* str, char const* sub_str, int start_pos);

	//��֣����ַ������ջ��ߣ���ֳ�����������,�����Ļ�������벿��
	bool split(const char* buf, const char* str_split, char* l, char* r, bool bcontain_str_split);

	// ���ַ�������ģ���и�������඼���������з��أ�������ģ��
	void parse_into_vector_array(const char* string_content, std::vector<std::string>& in_array, const char* parse_string);
}