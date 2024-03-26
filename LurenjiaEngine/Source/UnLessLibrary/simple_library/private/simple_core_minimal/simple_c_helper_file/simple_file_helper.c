// Copyright (C) RenZhai.2022.All Rights Reserved.
#include "../../../../simple_library/public/simple_core_minimal/simple_c_helper_file/simple_file_helper.h"
#include "../../../../simple_library/public/simple_core_minimal/simple_c_core/simple_c_array/simple_c_array_string.h"
#include "../../../../simple_library/public/simple_core_minimal/simple_c_core/simple_c_string_algorithm/string_algorithm.h"

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

//���ڼ��ShellExecute�ķ���ֵ��Ϣ
bool check_ShellExecute_ret(HINSTANCE ret)
{
	DWORDLONG result = (DWORDLONG)ret;
	if (result == 0)
	{
		// �ڴ治��
		assert("open_url_w=>insufficient memory.");
	}
	else if (result == 2)
	{
		// �ļ�������
		assert("open_url_w=>File name error.");
	}
	else if (result == 3)
	{
		// ·��������
		assert("open_url_w=>Path name error.");
	}
	else if (result == 11)
	{
		// EXE �ļ���Ч
		assert("open_url_w=>Invalid .exe file.");
	}
	else if (result == 26)
	{
		// �����������
		assert("open_url_w=>A sharing error occurred.");
	}
	else if (result == 27)
	{
		// �ļ�������ȫ����Ч
		assert("open_url_w=>incomplete or invalid file name.");
	}
	else if (result == 28)
	{
		// ��ʱ
		assert("open_url_w=>timeout.");
	}
	else if (result == 29)
	{
		// DDE ����ʧ��
		assert("open_url_w=> DDE transaction failed.");
	}
	else if (result == 30)
	{
		// ���ڴ������� DDE �����������ɸ� DDE ����
		assert("open_url_w=> is processing another DDE transaction and cannot complete the DDE transaction.");
	}
	else if (result == 31)
	{
		// û���������Ӧ�ó���
		assert("open_url_w=>no associated application.");
	}

	return result <= 32;
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
	//��ǰ�Ļ��� ����1MB��С����������ͻ������ �������std C�γ����������չ
	char Buf[1024 * 1024] = { 0 };
	int FileSize = 0;
	FILE *FpSrc = NULL;
	FILE *FpDest = NULL;

	if ((FpSrc = fopen(Src, "rb")) != NULL)
	{
		if ((FpDest = fopen(Dest, "wb")) != NULL)
		{
			while ((FileSize = (int)fread(Buf, 1, 512, FpSrc)) > 0)
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
	int my_len = (int)strlen(in_path) - 1;

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
			//���·�� ��һ��·��
			add_def_c_paths(str, buff);
		}

		//ƴ��
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
		(int)strlen(str) + 
		sizeof(char) + //�����ṩ��\0��β
		sizeof(char);//�������Դ���\0��β
}

int get_def_c_offset_w(const wchar_t* str)
{
	return
		(int)wcslen(str) +
		sizeof(wchar_t) + //�����ṩ��\0��β
		sizeof(wchar_t);//�������Դ���\0��β
}

const char* get_max_len_path(const def_c_paths *Paths,int *max_len)
{
	if (Paths)
	{
		const char* max_path = NULL;
		for (int i = 0; i < Paths->index; i++)
		{
			int paht_len = (int)strlen(Paths->paths[i]);
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

	//�ӳ���������
	for (int i = 0; i < Paths.index; i++)
	{
		// ע�������õ����ڿ���
		//int max_len = 0;
		//char* max_path = get_max_len_path(&Paths,&max_len);

		//if (max_path && max_path[0]!= '\0')
		//{
		//	//���·��
		//	add_def_c_paths(&tmp_paths, max_path);

		//	memset(max_path, 0, strlen(max_path));
		//}
	}

	int offset = 0;
	for (int i = 0; i < tmp_paths.num; i++)
	{
		char * in_tmp_path = get_def_c_paths_by_offset(&tmp_paths, offset);
		offset += get_def_c_offset(in_tmp_path);

		//if (in_tmp_path)
		//{
		//	wchar_t pathw[128] = { 0 };
		//	char_to_wchar_t(pathw, 128, in_tmp_path);
		//	RemoveDirectory((LPCSTR)pathw);
		//}
	}

	destroy_def_c_paths_v2(&tmp_paths);
}

void find_files(char const *in_path, def_c_paths *str, bool b_recursion,bool b_include_folder)
{
	char in_path_buff[512] = { 0 };

	strcpy(in_path_buff, in_path);
	int my_len = (int)strlen(in_path)-1;

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

		//ƴ��
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
	//���ַ�תΪխ�ַ�
	wchar_t path[1024] = { 0 };
	char_to_wchar_t(path,1024, url);

	return open_url_w(path);
}

bool open_url_by_param(const char* url, const char* param)
{
	//���ַ�תΪխ�ַ�
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

	//���ַ�תΪխ�ַ�
	wchar_t path[1024] = { 0 };
	char_to_wchar_t(path, 1024, url);

	wchar_t my_param[1024] = { 0 };
	char_to_wchar_t(my_param, 1024, param);

	return open_by_operation_w(my_operation,path, my_param);
}

bool open_explore(const char* url)
{
	//���ַ�תΪխ�ַ�
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
		while ((file_size = (int)fread(buf_tmp, 1,1024, f)) > 0)
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
		//���ļ�ָ���ƶ����ļ�β��;
		fseek(f, 0, SEEK_END);

		int l = 0;
		//�����ص�ǰ�ļ���λ�ã����ص�λΪ�ֽ� ����0 �����ļ���Ч
		if ((l = ftell(f)) > 0)
		{
			//���ļ�ָ���Ƶ��ļ�ͷ��
			rewind(f);
			//���ļ�����������
			fread(buf, sizeof(unsigned char), l, f);
			//�ļ�������Ҫ0��β
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
	//return check_ShellExecute_ret(ShellExecute(NULL,
	//	(LPCSTR)in_operation,
	//	(LPCSTR)url,
	//	(LPCSTR)param,
	//	NULL,
	//	SW_SHOWNORMAL));
	return false;
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
	int str_len = (int)strlen(str);
	str_len += sizeof(char);//���һ�� /0��β

	if (!c_paths->paths)
	{
		c_paths->paths = (char*)malloc(str_len);
	}
	else
	{
		c_paths->paths = (char*)realloc((void*)c_paths->paths, c_paths->index + str_len);
	}

	memset((void*)&c_paths->paths[c_paths->index], 0, str_len);

	memcpy(&c_paths->paths[c_paths->index++],str, str_len -sizeof(char));

	c_paths->index += str_len;
	c_paths->num++;
}

void add_def_c_paths_w(def_c_paths_w_v2* c_paths, const wchar_t* str)
{
	int str_len = (int)wcslen(str);
	str_len += sizeof(wchar_t);//���һ�� /0��β

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

		int len = (int)strlen(c_paths->paths); // (int)strlen(c_paths->paths[offset] ֮ǰ����仰�����£��������ٿ�
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

		int len = (int)wcslen(c_paths->paths);// (int)wcslen(c_paths->paths[offset]) ֮ǰ����仰�����£��������ٿ�
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
		//���ļ�ָ���ƶ����ļ�β��;
		fseek(f, 0, SEEK_END);

		int l = 0;
		//�����ص�ǰ�ļ���λ�ã����ص�λΪ�ֽ� ����0 �����ļ���Ч
		if ((l = ftell(f)) > 0)
		{
			//���ļ�ָ���Ƶ��ļ�ͷ��
			rewind(f);
			//���ļ�����������
			fread(buf, 1, l, f);
			//�ļ�������Ҫ0��β
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
	if ((f = fopen(path, "wb")) != NULL)
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
	size_t wchar_t_size = (int)wcslen(_Src);

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
	size_t char_size = (int)strlen(_Src);
	
	size_t char_to_wchar_t_count = 0;
	printf("\nchar to wchar_t:[%s];\n", strerror(mbstowcs_s(
		&char_to_wchar_t_count, 
		dst_wchar_t,
		wchar_t_size, 
		_Src,
		char_size)));
	
	return char_to_wchar_t_count;
}
