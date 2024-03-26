#pragma once
#include <vector>
#include <string>
#include <io.h>
#include <direct.h>

namespace helper_tool_files
{
	// �滻�ļ��е�\Ϊ/
	void normalization_path(char* path_buf);

	// �Ƿ����ģ�崮
	bool string_contain(const std::string& buff, const char* find_buff);
	// ��ǰ������ģ�崮
	int find_string(const char* str, char const* sub_str, int start_pos);
	// �Ӻ���ǰ��ģ�崮
	int find_string_from_end(const char* str, char const* sub_str, int start_pos);


	// ��ȡ�ļ���Сͨ���ļ�����
	unsigned int get_file_size_by_filename(const char* filename);
	// ��ȡ�ļ���Сͨ���ļ����
	unsigned int get_file_size(FILE* file_handle);
	// ��ȡ�ļ��ַ�ָ��
	bool get_file_buf(const char* path, char* buf);
	// ��ȡ�ļ���ÿһ�б��浽������
	bool load_file_to_strings(const std::string& in_path, std::vector<std::string>& out_array);
	//bool save_file_to_strings(const std::string& in_path, const std::vector<std::string>& int_array);
	//bool load_file_to_bytes(const std::string& in_path, std::vector<unsigned char>& out_array);
	//bool save_file_to_bytes(const std::string& in_path, const std::vector<unsigned char>& int_array);

	// ���� ���ַ������浽�ļ���
	bool save_file_to_strings(const std::string& in_path, const std::vector<std::string>& in_array);
	// �ļ��Ƿ����
	bool is_file_exists(char const* filename);
	// �����ļ���
	bool create_file_directory(char const* in_path);



	// �Ƴ��ַ����е����ַ���
	void remove_string_start(char* str, char const* sub_str);
	// �Ƴ��ַ�,��ǰ�������
	bool remove_char_start(char* str, char sub_str);
	// �Ƴ��ַ�,�Ӻ���ǰ����
	bool remove_char_end(char* str, char sub_str);
	// ȥ��ǰ��Ŀո�
	void trim_start_and_end_inline(char* buff);
	// ȥ��ǰ��Ŀո�
	void trim_start_inline(char* buff);
	// ȥ����ߵĿո�
	void trim_end_inline(char* buff);



	// ��֣����ַ������ջ��ߣ���ֳ�����������,�����Ļ�������벿��
	bool split(const char* buf, const char* str_split, char* l, char* r, bool bcontain_str_split);
	// ��֣����ַ�������ģ���и�������඼���������з��أ�������ģ��
	void parse_into_vector_array(const char* string_content, std::vector<std::string>& in_array, const char* parse_string);
}