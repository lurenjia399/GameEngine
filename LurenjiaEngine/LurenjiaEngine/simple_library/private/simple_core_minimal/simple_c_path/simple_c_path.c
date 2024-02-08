// Copyright (C) RenZhai.2022.All Rights Reserved.
#include "simple_library/public/simple_core_minimal/simple_c_path/simple_path.h"
#include "simple_library/public/simple_core_minimal/simple_c_core/simple_c_array/simple_c_array_string.h"

void get_path_directory_inline(char *path_buf)
{
	normalization_path(path_buf);// \\ /

	char filename[1024] = { 0 };
	get_path_clean_filename(filename, path_buf);

	remove_string_start(path_buf,filename);
}

void get_path_directory(char *buf, const char *path_buf)
{
	strcpy(buf,path_buf);
	get_path_directory_inline(buf);
}

void get_path_clean_filename(char *buf, const char *path_buf)
{
	simple_c_string c_string;
	if (strstr(path_buf,"\\"))
	{
		dismantling_string(path_buf, "\\", &c_string);
	}
	else if (strstr(path_buf, "/"))
	{
		dismantling_string(path_buf, "/", &c_string);
	}

	char* value = get_string(c_string.size - 1, &c_string);
	strcpy(buf, value);
	destroy_string(&c_string);
}

void normalization_path(char *path_buf)
{	
	//这种替换似乎有点问题 
	//replace_char_inline(path_buf,'\\','/');
	
	int len = strlen(path_buf);
	for (int i = 0; path_buf[i] != 0 && i < len; i++)
	{
		if (path_buf[i] == 92) 
		{
			path_buf[i] = '/';
		}
	}
}

void normalization_directory(char *buf, const char *path_buf)
{
	simple_c_string c_string;
	dismantling_string(path_buf, "/", &c_string);
	
	for (int i = 0 ; i < c_string.size - 1;i++)
	{
		char* value = get_string(i, &c_string);
		char buf_tmp[1024] = { 0 };
		strcat(buf_tmp, value);
		strcat(buf_tmp,"\\");
		strcat(buf, buf_tmp);
	}

	destroy_string(&c_string);
}

char* get_full_path(char* in_path_buf,int in_buff_len, const char* in_path)
{
#ifdef _WIN32
	_fullpath(in_path_buf, in_path, in_buff_len);
#else
	realpath(in_path, in_path_buf);
#endif
	normalization_path(in_path_buf);

	return in_path_buf;
}

void get_path_clean_filename_w(wchar_t* buf, const wchar_t* path_buf)
{
	simple_c_wstring c_string;
	if (wcsstr(path_buf, L"\\"))
	{
		dismantling_wstring(path_buf, L"\\", &c_string);
	}
	else if (wcsstr(path_buf, L"/"))
	{
		dismantling_wstring(path_buf, L"/", &c_string);
	}

	wchar_t* value = get_wstring(c_string.size - 1, &c_string);
	wcscpy(buf, value);
	destroy_wstring(&c_string);
}
