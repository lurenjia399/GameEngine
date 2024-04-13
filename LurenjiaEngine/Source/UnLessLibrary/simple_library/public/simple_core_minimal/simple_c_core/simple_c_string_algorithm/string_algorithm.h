#pragma once
//Copyright (C) RenZhai.2019.All Rights Reserved.

#include "../../../../public/simple_core_minimal/simple_c_core/simple_core_minimal.h"

_CRT_BEGIN_C_HEADER
void remove_string_start(char *str, char const* sub_str);

//�Ӻ���ǰ��
int find_string_from_end(const char* str, char const* sub_str, int start_pos);

//��ǰ������
int find_string(const char *str, char const* sub_str,int start_pos);

bool remove_char_start(char *str, char sub_str);

bool remove_char_end(char *str, char sub_str);

//�ж��ַ����Ƿ����
bool c_str_contain(const char* buff_str,const char *sub_str);

//ǰ��ո��޵�
void trim_start_and_end_inline(char* buff);

bool string_equal(const char* str_1, const char* str_2);

//ȥ��ǰ��Ŀո�
void trim_start_inline(char *buff);
//ȥ������Ŀո�
void trim_end_inline(char *buff);

//���
bool split(const char *buf,const char* str_split,char *l,char *r, bool bcontain_str_split);

void remove_all_char_end(char *str, char sub_str);

void replace_string_inline(char* str,const char* sub_char_a,const char* sub_char_b);
void replace_char_inline(char *str, const char sub_char_a, const char sub_char_b);

int get_printf(char *buf, const char *format, ...);

int get_printf_s(char *out_buf, const char *format, ...);

int get_printf_s_s(int buffer_size,char *out_buf, const char *format, ...);

char *string_mid(const char *int_buf ,char *out_buf,int start,int count);

int char_to_tchar(const char *str, wchar_t *tc);

int tchar_to_char(const wchar_t *tc, char *str);

//ע�� ��str �������㹻��Ŀռ� ��Ҫ��һ���Զ�ƥ���ڴ��ָ��
void wremove_string_start(wchar_t *str, wchar_t const* sub_str);

int wfind_string(wchar_t *str, wchar_t const* sub_str);

void  wremove_wchar_start(wchar_t *str, wchar_t sub_str);

void wremove_wchar_end(wchar_t *str, wchar_t sub_str);

void wremove_all_wchar_end(wchar_t *str, wchar_t sub_str);

void wreplace_wchar_inline(wchar_t *str, const wchar_t sub_char_a, const wchar_t sub_char_b);
void wreplace_string_inline(wchar_t* str, const wchar_t* sub_char_a, const wchar_t* sub_char_b);

int wget_printf(wchar_t *buf, const wchar_t *format, ...);

int wget_printf_s(wchar_t *out_buf, const wchar_t *format, ...);

int wget_printf_s_s(int buffer_size, wchar_t *out_buf,const wchar_t *format, ...);

wchar_t *wstring_mid(const wchar_t *int_buf, wchar_t *out_buf, int start, int count);

_CRT_END_C_HEADER