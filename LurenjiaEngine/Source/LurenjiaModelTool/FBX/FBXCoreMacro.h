#pragma once

// ��������Ŀ�����ж���LURENJIAMODELTOOL_API_EXPORTS����꣬��ǰ��Ŀ������dll��ʱ���dllexport����
// ������Ŀû������LURENJIAMODELTOOL_API_EXPORTS����꣬������ĿҲ�ͻ���__declspec(dllimport)����
// ���������static�Ķ���������Ҫ���룬�����ȫ�ֵľͲ���Ҫ����
// https://blog.csdn.net/Repeaterbin/article/details/4269666 ��������ʾ��

#ifdef LURENJIAMODELTOOL_API_EXPORTS
#define LURENJIAENMODELTOOL_API __declspec(dllexport)
#else
#define LURENJIAENMODELTOOL_API __declspec(dllimport)
#endif 