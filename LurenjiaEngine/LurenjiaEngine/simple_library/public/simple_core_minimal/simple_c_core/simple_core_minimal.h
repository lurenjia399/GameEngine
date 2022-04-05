#pragma once
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif // !1
#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif // !1

#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

#include <windows.h>
#include <vcruntime.h>
#include <assert.h>
#include <stdio.h>
#include <corecrt_wstdio.h>
#include <stdbool.h>
#include <corecrt_wstdio.h>
#include <stdarg.h>
#include <combaseapi.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <direct.h>
#include <time.h>
#include <wchar.h>
#include <process.h>
#include <shellapi.h>
#include <corecrt_wstring.h>

//#include <array>

#define SIMPLE_C_BUFF_SIZE 1024 //如果嫌小就给大点