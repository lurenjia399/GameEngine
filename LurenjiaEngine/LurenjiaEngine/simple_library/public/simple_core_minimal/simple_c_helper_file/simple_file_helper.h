#pragma once
//Copyright (C) RenZhai.2019.All Rights Reserved.

//��c�����ϸԴ�뽲����AboutCG ���ְ�������Ӳ��c���ԡ���Ƶ�������棺
//https://www.aboutcg.org/courseDetails/902/introduce
//ϣ��ѧϰ�������漼�� ������UE4��Ϸ, ���Է���������ַ��
//https://zhuanlan.zhihu.com/p/60117613
//
//bibi���Կ���������լϵ�н̳̽��ܣ�
//https://space.bilibili.com/29544409
//
//���˲�����վ
//http://renzhai.net
//
//���ڱ��װ�������ϸ���� :
//���ְ汾(��ϸ)��
//https://zhuanlan.zhihu.com/p/144558934
//��Ƶ�汾��
//https://www.bilibili.com/video/BV1x5411s7s3
#include "simple_library/public/simple_core_minimal/simple_c_core/simple_core_minimal.h"

_CRT_BEGIN_C_HEADER
#define _number_of_successful_conversions(a) a
#define _out_pram(a) a
#define _in_pram(a) a

typedef struct
{
	int index;
	char paths[2048][512];//MAX_PATH
}def_c_paths;

typedef struct
{
	int index;
	wchar_t paths[2048][512];//MAX_PATH
}def_c_paths_w;

void init_def_c_paths(def_c_paths *c_paths);
void init_def_c_paths_w(def_c_paths_w* c_paths);

int copy_file(char *Src, char *Dest);

//�Ƴ�Ŀ¼�������ļ�
void remove_dir_all_files(const char* file_dir);

//�ݹ��Ƴ��ļ�·�� ʹ�ø�APIȷ��·�����Ѿ�û���ļ� ֻ���ļ���
void remove_directory_all(const char* file_dir);

void find_files(char const *in_path, def_c_paths *str,bool b_recursion,bool b_include_folder);

bool is_file_exists(char const* filename);

bool create_file(char const *filename);
 
//����·��
bool create_file_directory(char const *in_path);

//�򿪵�ַ
bool open_url(const char* url);

//ͨ��������url
bool open_url_by_param(const char* url, const char* param);

//ͨ��������ĳ������
bool open_by_operation(const char* in_operation, const char* url, const char* param);

//��һ���ļ���
bool open_explore(const char* url);

//ʹ�øýӿ� һ��Ҫ��ʼ��buf
bool get_file_buf(const char *path,char *buf);

bool save_file_buff(const char* path, char* buf);

bool add_file_buf(const char *path, char *buf);

//������������ַ����ķ�ʽ�洢�������������0 �Զ��ضϣ������ö����ƴ洢
bool add_new_file_buf(const char *path, char *buf);

//������Զ����Ʒ�ʽ��ȡ
bool load_data_from_disk(const char* path, char* buf);

unsigned int get_file_size_by_filename(const char *filename);

unsigned int get_file_size(FILE *file_handle);

//������Զ����Ʒ�ʽ�洢������������0�Զ��ضϵ����
bool save_data_to_disk(const char* path, char* buf, int buf_size);
//���ַ���խ�ַ�
// 
//���ַ�תխ�ַ�
_number_of_successful_conversions(size_t) wchar_t_to_char(
	_out_pram(char*) dst_char,
	size_t char_size,
	_in_pram(wchar_t const*) _Src);

//խ�ַ�ת���ַ�
_number_of_successful_conversions(size_t) char_to_wchar_t(
	_out_pram(wchar_t*) dst_wchar_t,
	size_t wchar_t_size,
	_in_pram(char const*) _Src);

//���ַ�
//////////////////////////////////////////////
//������������ַ����ķ�ʽ�洢�������������0 �Զ��ضϣ������ö����ƴ洢
bool add_new_file_buf_w(const wchar_t* path, char* buf);

bool get_file_buf_w(const wchar_t* path, char* buf);

//������Զ����Ʒ�ʽ�洢������������0�Զ��ضϵ����
bool save_data_to_disk_w(const wchar_t* path, char* buf,int buf_size);

//������Զ����Ʒ�ʽ��ȡ buf�Ĵ�СҪ��ʵ�ʴ�С+1 ��Ϊ���һλ����/0
bool load_data_from_disk_w(const wchar_t* path, char* buf);

bool is_file_exists_w(const wchar_t *filename);

//�򿪵�ַ
bool open_url_w(const wchar_t* url);

bool open_url_by_param_w(const wchar_t* url,const wchar_t *param);

bool open_by_operation_w(const wchar_t *in_operation, const wchar_t* url, const wchar_t* param);

//��һ���ļ���
bool open_explore_w(const wchar_t* url);

unsigned int get_file_size_by_filename_w(const wchar_t* filename);

//v2�汾 ·������Ӧ

typedef struct
{
	int index;//��ʾ��ǰƫ��
	int num;//��ʾ����
	char* paths;//MAX_PATH
}def_c_paths_v2;

typedef struct
{
	int index;//��ʾ��ǰƫ��
	int num;//��ʾ����
	wchar_t* paths;//MAX_PATH
}def_c_paths_w_v2;

void init_def_c_paths_v2(def_c_paths_v2* c_paths);
void init_def_c_paths_w_v2(def_c_paths_w_v2* c_paths);

void find_files_v2(char const* in_path, def_c_paths_v2* str, bool b_recursion, bool b_include_folder);

int get_def_c_offset(const char* str);
int get_def_c_offset_w(const wchar_t* str);

void add_def_c_paths(def_c_paths_v2* c_paths,const char *str);
void add_def_c_paths_w(def_c_paths_w_v2* c_paths, const wchar_t* str);

int get_def_c_paths_offset_by_index(def_c_paths_v2* c_paths, int index);
int get_def_c_paths_offset_by_index_w(def_c_paths_w_v2* c_paths, int index);

char* get_def_c_paths_by_offset(def_c_paths_v2* c_paths, int in_offset);
wchar_t* get_def_c_paths_by_offset_w(def_c_paths_w_v2* c_paths, int in_offset);

char* get_def_c_paths_by_index(def_c_paths_v2* c_paths,int index);
wchar_t *get_def_c_paths_by_index_w(def_c_paths_w_v2* c_paths, int index);

void destroy_def_c_paths_v2(def_c_paths_v2* c_paths);
void destroy_def_c_paths_w_v2(def_c_paths_w_v2* c_paths);
_CRT_END_C_HEADER