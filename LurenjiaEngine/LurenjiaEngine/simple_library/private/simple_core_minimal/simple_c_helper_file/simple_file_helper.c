// Copyright (C) RenZhai.2022.All Rights Reserved.
#include "simple_library/public/simple_core_minimal/simple_c_helper_file/simple_file_helper.h"
#include "simple_library/public/simple_core_minimal/simple_c_core/simple_c_array/simple_c_array_string.h"
#include "simple_library/public/simple_core_minimal/simple_c_core/simple_c_string_algorithm/string_algorithm.h"

#ifdef  _WIN64
struct _finddatai64_t finddata;
char separator[] = "/";
char wildcard[] = "*";
#else
#ifdef _WIN32    
struct _finddata_t finddata;
char separator[] = "\\";
char wildcard[] = "*";
#endif 
#endif 

//用于检测ShellExecute的返回值信息
bool check_ShellExecute_ret(int ret)
{
	if (ret == 0)
	{
		// 内存不足
		assert(0, "open_url_w=>insufficient memory.");
	}
	else if (ret == 2)
	{
		// 文件名错误
		assert(0, "open_url_w=>File name error.");
	}
	else if (ret == 3)
	{
		// 路径名错误
		assert(0, "open_url_w=>Path name error.");
	}
	else if (ret == 11)
	{
		// EXE 文件无效
		assert(0, "open_url_w=>Invalid .exe file.");
	}
	else if (ret == 26)
	{
		// 发生共享错误
		assert(0, "open_url_w=>A sharing error occurred.");
	}
	else if (ret == 27)
	{
		// 文件名不完全或无效
		assert(0, "open_url_w=>incomplete or invalid file name.");
	}
	else if (ret == 28)
	{
		// 超时
		assert(0, "open_url_w=>timeout.");
	}
	else if (ret == 29)
	{
		// DDE 事务失败
		assert(0, "open_url_w=> DDE transaction failed.");
	}
	else if (ret == 30)
	{
		// 正在处理其他 DDE 事务而不能完成该 DDE 事务
		assert(0, "open_url_w=> is processing another DDE transaction and cannot complete the DDE transaction.");
	}
	else if (ret == 31)
	{
		// 没有相关联的应用程序
		assert(0, "open_url_w=>no associated application.");
	}

	return ret <= 32;
}

void init_def_c_paths(def_c_paths *c_paths)
{
	c_paths->index = 0;
	memset(c_paths->paths,0,sizeof(c_paths->paths) - 1);
}

void init_def_c_paths_w(def_c_paths_w* c_paths)
{
	c_paths->index = 0;
	memset(c_paths->paths, 0, sizeof(c_paths->paths) - 1);
}

int copy_file(char *Src, char *Dest)
{
	//当前的缓存 缓存1MB大小，如果超过就会出问题 这个会在std C课程里面继续扩展
	char Buf[1024 * 1024] = { 0 };
	int FileSize = 0;
	FILE *FpSrc = NULL;
	FILE *FpDest = NULL;

	if ((FpSrc = fopen(Src, "rb")) != NULL)
	{
		if ((FpDest = fopen(Dest, "wb")) != NULL)
		{
			while ((FileSize = fread(Buf, 1, 512, FpSrc)) > 0)
			{
				fwrite(Buf, FileSize, sizeof(char), FpDest);
				memset(Buf, 0, sizeof(Buf));
			}

			fclose(FpSrc);
			fclose(FpDest);

			return 0;
		}
	}

	return -1;
}

void remove_dir_all_files(const char* file_dir)
{
	if (!file_dir)
	{
		return;
	}

	def_c_paths Paths;
	init_def_c_paths(&Paths);

	find_files(file_dir, &Paths, true,false);

	for (int i = 0; i < Paths.index; i++)
	{
		remove(Paths.paths[i]);
	}
}

void find_files_v2(char const* in_path, def_c_paths_v2* str, bool b_recursion, bool b_include_folder)
{
	char in_path_buff[512] = { 0 };

	strcpy(in_path_buff, in_path);
	int my_len = strlen(in_path) - 1;

	if (my_len > 0)
	{
		if (in_path_buff[my_len] == '/')
		{
			remove_char_end(in_path_buff, '/');
		}

#ifdef  _WIN64
		intptr_t hfile = 0;
#else
#ifdef _WIN32    
		long hfile = 0;
#endif 
#endif 
		char buff[1024] = { 0 };

		strcpy(buff, in_path_buff);
		strcat(buff, separator);

		if (b_include_folder)
		{
			//添加路径 第一个路径
			add_def_c_paths(str, buff);
		}

		//拼接
		strcat(buff, wildcard);
		if ((hfile =
#ifdef _WIN64
			_findfirst64
#else
#ifdef WIN32	
			_findfirst
#endif // _WIN64
#endif // _WIN32
			(buff, &finddata)) != -1)
		{
			do
			{
				if (finddata.attrib & _A_SUBDIR)
				{
					if (strcmp(finddata.name, ".") == 0 ||
						strcmp(finddata.name, "..") == 0)
					{
						continue;
					}

					memset(buff, 0, 1024);

					strcpy(buff, in_path_buff);
					strcat(buff, separator);
					strcat(buff, finddata.name);

					if (b_recursion)
					{
						find_files_v2(buff, str, b_recursion, b_include_folder);
					}
				}
				else
				{
					memset(buff, 0, 1024);

					strcpy(buff, in_path_buff);
					strcat(buff, separator);
					strcat(buff, finddata.name);

					add_def_c_paths(str, buff);
				}

			} while (
#ifdef _WIN64
				_findnext64
#else
#ifdef _WIN32

				_findnext
#endif
#endif
				(hfile, &finddata) == 0);
			_findclose(hfile);
		}
	}
}

int get_def_c_offset(const char* str)
{
	return 
		strlen(str) + 
		sizeof(char) + //我们提供的\0结尾
		sizeof(char);//拷贝中自带的\0结尾
}

int get_def_c_offset_w(const wchar_t* str)
{
	return
		wcslen(str) +
		sizeof(wchar_t) + //我们提供的\0结尾
		sizeof(wchar_t);//拷贝中自带的\0结尾
}

char* get_max_len_path(const def_c_paths *Paths,int *max_len)
{
	if (Paths)
	{
		char* max_path = NULL;
		for (int i = 0; i < Paths->index; i++)
		{
			int paht_len = strlen(Paths->paths[i]);
			if (paht_len > *max_len)
			{
				*max_len = paht_len;
				max_path = Paths->paths[i];
			}
		}

		return max_path;
	}

	return NULL;
}

void remove_directory_all(const char* file_dir)
{
	def_c_paths Paths;
	init_def_c_paths(&Paths);

	find_files(file_dir, &Paths, true, true);

	def_c_paths_v2 tmp_paths;
	init_def_c_paths_v2(&tmp_paths);

	//从长到短排列
	for (int i = 0; i < Paths.index; i++)
	{
		int max_len = 0;
		char* max_path = get_max_len_path(&Paths,&max_len);

		if (max_path && max_path[0]!= '\0')
		{
			//添加路径
			add_def_c_paths(&tmp_paths, max_path);

			memset(max_path, 0, strlen(max_path));
		}
	}

	int offset = 0;
	for (int i = 0; i < tmp_paths.num; i++)
	{
		char * in_tmp_path = get_def_c_paths_by_offset(&tmp_paths, offset);
		offset += get_def_c_offset(in_tmp_path);

		if (in_tmp_path)
		{
			wchar_t pathw[128] = { 0 };
			char_to_wchar_t(pathw, 128, in_tmp_path);
			RemoveDirectory(pathw);
		}
	}

	destroy_def_c_paths_v2(&tmp_paths);
}

void find_files(char const *in_path, def_c_paths *str, bool b_recursion,bool b_include_folder)
{
	char in_path_buff[512] = { 0 };

	strcpy(in_path_buff, in_path);
	int my_len = strlen(in_path)-1;

	if (my_len > 0)
	{
		if (in_path_buff[my_len] == '/')
		{
			remove_char_end(in_path_buff, '/');
		}

#ifdef  _WIN64
		intptr_t hfile = 0;
#else
#ifdef _WIN32    
		long hfile = 0;
#endif 
#endif 

		char tmp_path[1024] = { 0 };
		strcpy(tmp_path, in_path_buff);

		if (b_include_folder)
		{
			strcpy(str->paths[str->index], in_path_buff);
			strcat(str->paths[str->index++], separator);
		}

		//拼接
		strcat(tmp_path, separator);
		strcat(tmp_path, wildcard);
		if ((hfile =
#ifdef _WIN64
			_findfirst64
#else
#ifdef WIN32	
			_findfirst
#endif // _WIN64
#endif // _WIN32
			(tmp_path, &finddata)) != -1)
		{
			do
			{
				if (finddata.attrib & _A_SUBDIR)
				{
					if (strcmp(finddata.name, ".") == 0 ||
						strcmp(finddata.name, "..") == 0)
					{
						continue;
					}

					char new_path[1024] = { 0 };
					strcpy(new_path, in_path_buff);
					strcat(new_path, separator);
					strcat(new_path, finddata.name);

					if (b_recursion)
					{
						find_files(new_path, str, b_recursion, b_include_folder);
					}
				}
				else
				{
					strcpy(str->paths[str->index], in_path_buff);
					strcat(str->paths[str->index], separator);
					strcat(str->paths[str->index++], finddata.name);
				}

			} while (
#ifdef _WIN64
				_findnext64
#else
#ifdef _WIN32

				_findnext
#endif
#endif
				(hfile, &finddata) == 0);
			_findclose(hfile);
		}
	}
}

bool is_file_exists(char const* filename)
{
	FILE* file = fopen(filename, "r");
	if(file)
	{
		fclose(file);
		return true;
	}
	return false;
}

bool create_file(char const *filename)
{
	FILE *f = NULL;
	if ((f = fopen(filename,"w+")) != NULL)
	{
		fclose(f);

		return true;
	}

	return false;
}

bool create_file_directory(char const *in_path)
{
	simple_c_string c_file;
	if (strstr(in_path, "\\"))
	{
		dismantling_string(in_path, "\\", &c_file);
	}
	else if (strstr(in_path, "/"))
	{
		dismantling_string(in_path, "/", &c_file);
	}

	char path[260] = { 0 };
	for (int i = 0;i < c_file.size;i++)
	{
		char *value = get_string(i,&c_file);
		strcat(value, "\\");
		strcat(path, value);
		if (_access(path,0) == -1)
		{
			_mkdir(path);
		}
	}

	destroy_string(&c_file);

	return _access(path, 0) == 0;
}

bool open_url(const char* url)
{
	//宽字符转为窄字符
	wchar_t path[1024] = { 0 };
	char_to_wchar_t(path,1024, url);

	return open_url_w(path);
}

bool open_url_by_param(const char* url, const char* param)
{
	//宽字符转为窄字符
	wchar_t path[1024] = { 0 };
	char_to_wchar_t(path, 1024, url);

	wchar_t my_param[1024] = { 0 };
	char_to_wchar_t(my_param, 1024, param);
	return open_url_by_param_w(path, my_param);
}

bool open_by_operation(const char* in_operation, const char* url, const char* param)
{
	wchar_t my_operation[1024] = { 0 };
	char_to_wchar_t(my_operation, 1024, in_operation);

	//宽字符转为窄字符
	wchar_t path[1024] = { 0 };
	char_to_wchar_t(path, 1024, url);

	wchar_t my_param[1024] = { 0 };
	char_to_wchar_t(my_param, 1024, param);

	return open_by_operation_w(my_operation,path, my_param);
}

bool open_explore(const char* url)
{
	//宽字符转为窄字符
	wchar_t path[1024] = { 0 };
	char_to_wchar_t(path, 1024, url);

	return open_explore_w(path);
}

bool get_file_buf(const char *path, char *buf)
{
	FILE *f = NULL;
	if ((f = fopen(path, "r")) != NULL)
	{
		char buf_tmp[2048] = { 0 };
		int file_size = 0;
		while ((file_size = fread(buf_tmp, 1,1024, f)) > 0)
		{
			strcat(buf, buf_tmp);
			memset(buf_tmp, 0, sizeof(buf_tmp));
		}

		fclose(f);

		return buf[0] != '\0';
	}

	return false;
}

bool save_file_buff(const char* path, char* buf)
{
	FILE* f = NULL;
	if ((f = fopen(path, "w")) != NULL)
	{
		fprintf(f, "%s", buf);
		fclose(f);

		return true;
	}

	return false;
}

bool add_file_buf(const char *path, char *buf)
{
	FILE *f = NULL;
	if ((f = fopen(path, "a+")) != NULL)
	{
		fprintf(f, "%s", buf);
		fclose(f);

		return true;
	}

	return false;
}

bool add_new_file_buf(const char *path, char *buf)
{
	FILE *f = NULL;
	if ((f = fopen(path, "w+")) != NULL)
	{
		fprintf(f, "%s", buf);
		fclose(f);

		return true;
	}

	return false;
}

bool add_new_file_buf_w(const wchar_t* path, char* buf)
{
	FILE* f = NULL;
	if ((f = _wfopen(path, L"w+")) != NULL)
	{
		fprintf(f, "%s", buf);
		fclose(f);

		return true;
	}

	return false;
}

bool get_file_buf_w(const wchar_t* path, char* buf)
{
	FILE* f = NULL;
	if ((f = _wpopen(path, L"r")) != NULL)
	{
		char buf_tmp[2048] = { 0 };
		int file_size = 0;
		while ((file_size = fread(buf_tmp, 1, 1024, f)) > 0)
		{
			strcat(buf, buf_tmp);
			memset(buf_tmp, 0, sizeof(buf_tmp));
		}

		fclose(f);

		return buf[0] != '\0';
	}

	return false;
}

bool save_data_to_disk_w(const wchar_t* path, char* buf, int buf_size)
{
	FILE* f = NULL;
	if ((f = _wfopen(path, L"w+")) != NULL)
	{
		fwrite(buf, buf_size, 1, f);
		fclose(f);
		return true;
	}

	return false;
}

bool load_data_from_disk_w(const wchar_t* path, char* buf)
{
	FILE* f = NULL;
	if ((f = _wfopen(path, L"rb")) != NULL)
	{
		//把文件指针移动到文件尾部;
		fseek(f, 0, SEEK_END);

		int l = 0;
		//来返回当前文件的位置，返回单位为字节 大于0 代表文件有效
		if ((l = ftell(f)) > 0)
		{
			//把文件指针移到文件头部
			rewind(f);
			//把文件拷贝到缓存
			fread(buf, sizeof(unsigned char), l, f);
			//文件后面需要0结尾
			//buf[l] = '\0';
		}
		fclose(f);

		return true;
	}

	return false;
}

bool is_file_exists_w(const wchar_t* filename)
{
	FILE* f = NULL;
	if ((f = _wfopen(filename, L"r")) != NULL)
	{
		fclose(f);

		return true;
	}

	return false;
}

bool open_url_w(const wchar_t* url)
{
	return open_by_operation_w(L"open", url, NULL);;
}

bool open_url_by_param_w(const wchar_t* url, const wchar_t* param)
{
	return open_by_operation_w(L"open", url, param);;
}

bool open_by_operation_w(const wchar_t* in_operation, const wchar_t* url, const wchar_t* param)
{
	return check_ShellExecute_ret(ShellExecute(NULL,
		in_operation,
		url,
		param,
		NULL,
		SW_SHOWNORMAL));
}

bool open_explore_w(const wchar_t* url)
{
	return open_by_operation_w(L"explore", url,NULL);;
}

unsigned int get_file_size_by_filename_w(const wchar_t* filename)
{
	unsigned int file_size = 0;

	FILE* f = NULL;
	if ((f = _wfopen(filename, L"r")) != NULL)
	{
		file_size = get_file_size(f);

		fclose(f);
	}

	return file_size;
}

void init_def_c_paths_v2(def_c_paths_v2* c_paths)
{
	c_paths->index = 0;
	c_paths->num = 0;
	c_paths->paths = NULL;
}

void init_def_c_paths_w_v2(def_c_paths_w_v2* c_paths)
{
	c_paths->index = 0;
	c_paths->num = 0;
	c_paths->paths = NULL;
}

void add_def_c_paths(def_c_paths_v2* c_paths, const char* str)
{
	int str_len = strlen(str);
	str_len += sizeof(char);//最后一个 /0结尾

	if (!c_paths->paths)
	{
		c_paths->paths = (char*)malloc(str_len);
	}
	else
	{
		c_paths->paths = (char*)realloc(c_paths->paths, c_paths->index + str_len);
	}

	memset(&c_paths->paths[c_paths->index], 0, str_len);

	memcpy(&c_paths->paths[c_paths->index++],str, str_len -sizeof(char));

	c_paths->index += str_len;
	c_paths->num++;
}

void add_def_c_paths_w(def_c_paths_w_v2* c_paths, const wchar_t* str)
{
	int str_len = wcslen(str);
	str_len += sizeof(wchar_t);//最后一个 /0结尾

	if (!c_paths->paths)
	{
		c_paths->paths = (wchar_t*)malloc(str_len);
	}
	else
	{
		c_paths->paths = (wchar_t*)realloc(c_paths->paths, c_paths->index + str_len);
	}

	memset(&c_paths->paths[c_paths->index], 0, str_len);

	memcpy(&c_paths->paths[c_paths->index], str, str_len- sizeof(wchar_t));

	c_paths->index += str_len;
	c_paths->num++;
}

int get_def_c_paths_offset_by_index(def_c_paths_v2* c_paths, int index)
{
	int offset = 0;
	for (int i = 0; i < c_paths->num; i++)
	{
		if (i == index)
		{
			return offset;
		}

		int len = strlen(c_paths->paths[offset]);
		offset += len + sizeof(char);
	}

	return -1;
}

int get_def_c_paths_offset_by_index_w(def_c_paths_w_v2* c_paths, int index)
{
	int offset = 0;
	for (int i = 0; i < c_paths->num; i++)
	{
		if (i == index)
		{
			return offset;
		}

		int len = wcslen(c_paths->paths[offset]);
		offset += len + sizeof(wchar_t);
	}

	return -1;
}

char* get_def_c_paths_by_offset(def_c_paths_v2* c_paths, int in_offset)
{
	return &c_paths->paths[in_offset];
}

wchar_t* get_def_c_paths_by_offset_w(def_c_paths_w_v2* c_paths, int in_offset)
{
	return &c_paths->paths[in_offset];
}

char *get_def_c_paths_by_index(def_c_paths_v2* c_paths, int index)
{
	int offset = get_def_c_paths_offset_by_index(c_paths, index);

	if (offset != -1)
	{
		return get_def_c_paths_by_offset(c_paths, offset);
	}

	return NULL;
}

wchar_t *get_def_c_paths_by_index_w(def_c_paths_w_v2* c_paths, int index)
{
	int offset = get_def_c_paths_offset_by_index_w(c_paths, index);

	if (offset != -1)
	{
		return get_def_c_paths_by_offset_w(c_paths, offset);
	}

	return NULL;
}

void destroy_def_c_paths_v2(def_c_paths_v2* c_paths)
{
	free(c_paths->paths);
	init_def_c_paths_v2(c_paths);
}

void destroy_def_c_paths_w_v2(def_c_paths_w_v2* c_paths)
{
	free(c_paths->paths);
	init_def_c_paths_w_v2(c_paths);
}

bool load_data_from_disk(const char* path, char* buf)
{
	FILE* f = NULL;
	if ((f = fopen(path, "rb")) != NULL)
	{
		//把文件指针移动到文件尾部;
		fseek(f, 0, SEEK_END);

		int l = 0;
		//来返回当前文件的位置，返回单位为字节 大于0 代表文件有效
		if ((l = ftell(f)) > 0)
		{
			//把文件指针移到文件头部
			rewind(f);
			//把文件拷贝到缓存
			fread(buf, 1, l, f);
			//文件后面需要0结尾
			//buf[l] = '\0';
		}
		fclose(f);

		return true;
	}

	return false;
}

unsigned int get_file_size_by_filename(const char *filename)
{
	unsigned int file_size = 0;

	FILE *f = NULL;
	if ((f = fopen(filename,"r")) != NULL)
	{
		file_size = get_file_size(f);

		fclose(f);
	}

	return file_size;
}

//asdoiajoi ajs aoisjd oaisjd oiasjdoi asodao ijaosijd oaisdja index
unsigned int get_file_size(FILE *file_handle)
{
	unsigned int file_size = 0;

	unsigned int current_read_postion = ftell(file_handle);
	fseek(file_handle, 0, SEEK_END);
	file_size = ftell(file_handle);
	fseek(file_handle, current_read_postion, SEEK_SET);

	return file_size;
}

bool save_data_to_disk(const char* path, char* buf, int buf_size)
{
	FILE* f = NULL;
	if ((f = fopen(path, L"wb")) != NULL)
	{
		fwrite(buf, buf_size, 1, f);
		fclose(f);
		return true;
	}

	return false;
}

size_t wchar_t_to_char(
	_out_pram(char*) dst_char,
	size_t char_size,
	_in_pram(wchar_t const*) _Src)
{
	size_t wchar_t_size = wcslen(_Src);

	size_t wchar_t_to_char_count = 0;
	printf("\nwchar_t to char:[%s];\n", strerror(wcstombs_s(
		&wchar_t_to_char_count, 
		dst_char, char_size, 
		_Src,
		wchar_t_size)));

	return wchar_t_to_char_count;
}

size_t char_to_wchar_t(
	_out_pram(wchar_t*) dst_wchar_t,
	size_t wchar_t_size, 
	_in_pram(char const*) _Src)
{
	size_t char_size = strlen(_Src);
	
	size_t char_to_wchar_t_count = 0;
	printf("\nchar to wchar_t:[%s];\n", strerror(mbstowcs_s(
		&char_to_wchar_t_count, 
		dst_wchar_t,
		wchar_t_size, 
		_Src,
		char_size)));
	
	return char_to_wchar_t_count;
}
