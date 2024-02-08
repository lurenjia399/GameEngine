#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif // !_CRT_SECURE_NO_WARNINGS

#include "simple_library/public/simple_cpp_core_minimal/simple_cpp_string_algorithm/simple_cpp_string_algorithm.h"
#include "simple_library/public/simple_core_minimal/simple_c_core/simple_c_string_algorithm/string_algorithm.h"

namespace simple_cpp_string_algorithm
{
	struct fframe_buffer_index
	{
		fframe_buffer_index()
			:strat_pos(0)
			,offset(0)
		{}

		int strat_pos;
		int offset;
	};

	bool string_contain(const std::string& buff,const char* find_buff)
	{
		return buff.find(find_buff) != -1;
	}

	void parse_into_vector_array(
		const char* string_content, 
		std::vector<std::string>& in_array,
		const char* parse_string)
	{
		int string_content_len = strlen(string_content);
		int parse_string_len = strlen(parse_string);

		fframe_buffer_index frame_buffer;
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
	bool index_valid(int in_size, int index)
	{
		if (index < in_size &&
			index >= 0)
		{
			return true;
		}

		return false;
	}

	std::string printf(const char* format, ...)
	{
		char buf[STRING_SIZE_PRINTF] = { 0 };
		memset(buf, 0, sizeof(char) * STRING_SIZE_PRINTF);
		va_list args;
		va_start(args, format);
		_vsnprintf_s(buf, STRING_SIZE_PRINTF - 1, STRING_SIZE_PRINTF, format, args);
		va_end(args);
		buf[STRING_SIZE_PRINTF - 1] = 0;

		char out_buf[STRING_SIZE_PRINTF] = { 0 };
		strcat(out_buf, buf);

		return std::string(out_buf);
	}
}