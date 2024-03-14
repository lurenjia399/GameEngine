#pragma once
// Copyright (C) RenZhai.2022.All Rights Reserved.

#include "../../../public/simple_core_minimal/simple_c_core/simple_core_minimal.h"

_CRT_BEGIN_C_HEADER
void get_path_directory_inline(char *path_buf);
void get_path_directory(char *buf, const char *path_buf);
void normalization_path(char *path_buf);
void get_path_clean_filename(char *buf, const char *path_buf);
void normalization_directory(char *buf, const char *path_buf);

char* get_full_path(char* in_path_buf, int in_buff_len,const char *in_path);

void get_path_clean_filename_w(wchar_t* buf, const wchar_t* path_buf);
_CRT_END_C_HEADER
