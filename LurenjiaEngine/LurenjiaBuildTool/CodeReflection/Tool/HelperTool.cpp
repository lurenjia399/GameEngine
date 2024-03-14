#include "HelperTool.h"

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

unsigned int helper_tool_files::get_file_size_by_filename(const char* filename)
{
	unsigned int file_size = 0;

	FILE* f = NULL;
	if ((f = fopen(filename, "r")) != NULL)
	{
		file_size = get_file_size(f);

		fclose(f);
	}

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
	FILE* f = NULL;
	if ((f = fopen(path, "r")) != NULL)
	{
		char buf_tmp[2048] = { 0 };
		int file_size = 0;
		while ((file_size = (int)fread(buf_tmp, 1, 1024, f)) > 0)
		{
			strcat(buf, buf_tmp);
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
		strcpy(&str[index], &str[index + (int)strlen(sub_str)]);
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

		strncpy(l, buf, pos);

		int nest_pos = pos + str_split_len;
		strncpy(r, &buf[nest_pos], buf_len - nest_pos);

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
		in_array.push_back(string_content);
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
				strncpy(in_ptr, &string_content[frame_buffer.strat_pos], frame_buffer.offset);
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