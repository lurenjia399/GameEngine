#include "HelperTool.h"
#include <stdio.h>

bool helper_tool_files::string_contain(const std::string& buff, const char* find_buff)
{
	return buff.find(find_buff) != -1;
}

bool helper_tool_files::remove_char_end(char* str, char sub_str)
{
	int len = (int)strlen(str);

	for (int i = len; i >= 0; i--)
	{
		if (str[i] == sub_str)
		{
			//strcpy(&str[i], &str[i + 1]);
			strcpy_s(&str[i], len - i, &str[i + 1]);
			return true;
		}
	}

	return false;
}

bool helper_tool_files::load_file_to_strings(const std::string& in_path, std::vector<std::string>& out_array)
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


						//char* content = strncpy(pre_line,&file_buff[start_offset + end_str_line_offset],len);
						strncpy_s(pre_line, len + 1,&file_buff[start_offset + end_str_line_offset],len);


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

unsigned int helper_tool_files::get_file_size_by_filename(const char* filename)
{
	unsigned int file_size = 0;

	FILE* f = nullptr;
	fopen_s(&f, filename, "r");
	
	if (f)
	{
		file_size = get_file_size(f);
		fclose(f);
	}
	//if ((fopen_s(&f, filename, "r")) != NULL)
	//{
	//	

	//	
	//}
	//else
	//{
	//	errNum = errno;

	//	//size_t errmsglen = strlen(errno) + 1;
	//	//char errmsg[errmsglen];
	//	//strerror_s(errmsg, errmsglen, errno);
	//	/*
	//		#define EPERM 1  Operation not permitted 
	//		#define ENOENT 2  No such file or directory 
	//		#define ESRCH 3  No such process 
	//		#define EIO 5  I/O error 
	//		#define ENXIO 6  No such device or address 
	//		#define E2BIG 7  Argument list too long 
	//		#define ENOEXEC 8  Exec format error *
	//		#define EBADF 9  Bad file number 
	//		#define ECHILD 10  No child processes 
	//	*/
	//	printf("open fail errno = %d reason = \n", errNum);
	//	printf("open fail errno = %d reason = \n", errNum);
	//}

	return file_size;
}

unsigned int helper_tool_files::get_file_size(FILE* file_handle)
{
	unsigned int file_size = 0;

	unsigned int current_read_postion = ftell(file_handle);
	fseek(file_handle, 0, SEEK_END);
	file_size = ftell(file_handle);
	fseek(file_handle, current_read_postion, SEEK_SET);

	return file_size;
}

bool helper_tool_files::get_file_buf(const char* path, char* buf)
{
	FILE* f = nullptr;
	fopen_s(&f, path, "r");
	if (f != nullptr)
	{
		char buf_tmp[2048] = { 0 };
		int file_size = 0;
		while ((file_size = (int)fread(buf_tmp, 1, 1024, f)) > 0)
		{
			int len = (int)strlen(buf) + (int)strlen(buf_tmp) + 1;
			strcat_s(buf, len, buf_tmp);
			memset(buf_tmp, 0, sizeof(buf_tmp));
		}

		fclose(f);

		return buf[0] != '\0';
	}

	return false;
}

int helper_tool_files::find_string(const char* str, char const* sub_str, int start_pos)
{
	int len = (int)strlen(sub_str);
	int index = -1;
	for (int i = start_pos; str[i] != '\0'; i++)
	{
		if (sub_str[0] == str[i])
		{
			int tmp_index = i;
			int l = 1;//第一个是成功
			while (sub_str[l] == str[i + l] && sub_str[l] != '\0')
			{
				l++;
			}

			if (len == l)
			{
				index = tmp_index;
				break;
			}
		}
	}

	return index;
}


void helper_tool_files::remove_string_start(char* str, char const* sub_str)
{
	int index = find_string(str, sub_str, 0);
	if (index != -1)
	{
		int sub_str_len = (int)strlen(sub_str);
		int str_len = (int)strlen(str);
		int len = str_len - (index + sub_str_len) + 1;
		strcpy_s(&str[index], len, &str[index + (int)strlen(sub_str)]);
	}
}

void helper_tool_files::trim_start_and_end_inline(char* buff)
{
	trim_start_inline(buff);
	trim_end_inline(buff);
}

void helper_tool_files::trim_start_inline(char* buff)
{
	int pos = find_string(buff, " ", 0);
	int index = 0;
	if (pos == 0)
	{
		while (pos == index)
		{
			index++;
			pos = find_string(buff, " ", index);
		}

		int buff_len = (int)strlen(buff);
		for (int i = index; i < buff_len; i++)
		{
			buff[i - index] = buff[i];
		}

		//填充最后 0
		for (int i = 1; i <= index; i++)
		{
			buff[buff_len - i] = '\0';
		}
	}
}
void helper_tool_files::trim_end_inline(char* buff)
{
	int buff_len = (int)strlen(buff);
	int pos = find_string_from_end(buff, " ", 0);
	int index = 1;

	if (buff_len > 0)
	{
		if (pos == (buff_len - index))
		{
			buff[buff_len - index] = '\0';

			trim_end_inline(buff);
		}
	}
}

int helper_tool_files::find_string_from_end(const char* str, char const* sub_str, int start_pos)
{
	int str_len = (int)strlen(str);
	int len = (int)strlen(sub_str);
	int index = -1;
	for (int i = (str_len - start_pos); str[i] != str[0]; i--)
	{
		if (sub_str[0] == str[i])
		{
			int tmp_index = i;
			int l = 1;//第一个是成功
			while (sub_str[l] == str[i + l] && sub_str[l] != '\0')
			{
				l++;
			}

			if (len == l)
			{
				index = tmp_index;
				break;
			}
		}
	}

	return index;
}

bool helper_tool_files::split(const char* buf, const char* str_split, char* l, char* r, bool bcontain_str_split)
{
	int pos = find_string(buf, str_split, 0);
	if (pos != -1)
	{
		int str_split_len = 0;
		if (!bcontain_str_split)
		{
			str_split_len = (int)strlen(str_split);
		}

		int buf_len = (int)strlen(buf);

		//strncpy(l, buf, pos);
		strncpy_s(l, pos + 1, buf, pos);

		int nest_pos = pos + str_split_len;
		//strncpy(r, &buf[nest_pos], buf_len - nest_pos);
		strncpy_s(r, buf_len - nest_pos + 1, &buf[nest_pos], buf_len - nest_pos);

		return true;
	}

	return false;
}

void helper_tool_files::parse_into_vector_array(const char* string_content,std::vector<std::string>& in_array,const char* parse_string)
{
	struct fframe_buffer_index
	{
		fframe_buffer_index()
			:strat_pos(0)
			, offset(0)
		{}

		int strat_pos;
		int offset;
	};

	int string_content_len = (int)strlen(string_content);
	int parse_string_len = (int)strlen(parse_string);

	fframe_buffer_index frame_buffer = {};
	int pos = find_string(string_content, parse_string, frame_buffer.strat_pos);

	if (pos == -1)
	{
		if (string_content_len != 0)
		{
			in_array.push_back(string_content);
		}
	}
	else
	{
		auto handle_string = [&]()
			{
				in_array.push_back(std::string());
				std::string& in_string = in_array[in_array.size() - 1];
				in_string.resize(frame_buffer.offset);
				char* in_ptr = const_cast<char*>(in_string.c_str());

				//数据拷贝
				//strncpy(in_ptr, &string_content[frame_buffer.strat_pos], frame_buffer.offset);
				strncpy_s(in_ptr, frame_buffer.offset + 1, &string_content[frame_buffer.strat_pos], frame_buffer.offset);
			};

		bool bloop = false;
		while (pos != -1)
		{
			frame_buffer.offset = pos - frame_buffer.strat_pos;

			handle_string();

			frame_buffer.strat_pos = pos + parse_string_len;
			pos = find_string(string_content, parse_string, frame_buffer.strat_pos);

			bloop = true;
		}

		//存在最后一次
		if (bloop)
		{
			frame_buffer.offset = string_content_len - frame_buffer.strat_pos;
			handle_string();
		}
	}
}

bool helper_tool_files::remove_char_start(char* str, char sub_str)
{
	int len = (int)strlen(str) + 1;

	//从头开始检查 然后删除对象
	for (int i = 0; i < len; i++)
	{
		if (str[i] == sub_str)
		{
			do
			{
				str[i] = str[i + 1];
				i++;
			} while (str[i + 1] != '\0');
			str[i] = '\0';

			return true;
		}
	}

	return false;
}

void helper_tool_files::normalization_path(char* path_buf)
{
	int len = (int)strlen(path_buf);
	for (int i = 0; path_buf[i] != 0 && i < len; i++)
	{
		if (path_buf[i] == 92) // 字符'\'
		{
			path_buf[i] = '/';
		}
	}
}

bool helper_tool_files::save_file_to_strings(const std::string& in_path, const std::vector<std::string>& in_array)
{

	if (!is_file_exists(in_path.c_str()))
	{

		char path_directory[1024] = { 0 };
		
		//strcpy(path_directory, in_path.c_str());

		if (create_file_directory(path_directory))
		{
			// 爆红先注掉
			//create_file(in_path.c_str());
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
		//if (simple_cpp_string_algorithm::index_valid((int)final_str.size(), start_pos))
		//{
		//	char* ptr = &final_str[start_pos];
		//	strcpy(ptr, tmp.c_str());

		//	int offset = 0;
		//	if (ptr[tmp_str_len - 1] == '\0')
		//	{
		//		int index = 1;
		//		//check
		//		while (ptr[tmp_str_len - index] == '\0')
		//		{
		//			index++;
		//		}

		//		for (int i = index; i > 0; i--)
		//		{
		//			ptr[tmp_str_len - i] = ' ';
		//		}

		//		ptr[tmp_str_len - index] = '\n';
		//	}
		//	else
		//	{
		//		offset++;
		//		ptr[tmp_str_len] = '\n';
		//	}

		//	start_pos += (tmp_str_len + offset);
		//}
	}

	final_str[final_num] = '\0';

	char* buff = const_cast<char*>(final_str.c_str());

	//return save_file_buff(in_path.c_str(), buff);;
	return false;
}

bool helper_tool_files::is_file_exists(char const* filename)
{
	//FILE* file = fopen(filename, "r");
	//if (file)
	//{
		//fclose(file);
		//return true;
	//}
	return false;
}

bool helper_tool_files::create_file_directory(char const* in_path)
{
	std::string c_file;

	char path[260] = { 0 };
	for (int i = 0; i < c_file.size(); i++)
	{
		char* value = &c_file[i];
		//strcat(value, "\\");
		//strcat(path, value);
		if (_access(path, 0) == -1)
		{
			_mkdir(path);
		}
	}

	//destroy_string(&c_file);

	return _access(path, 0) == 0;
}