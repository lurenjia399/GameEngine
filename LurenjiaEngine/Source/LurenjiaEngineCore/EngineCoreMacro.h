#pragma once

// ��������Ŀ�����ж���LURENJIAMODELTOOL_API_EXPORTS����꣬��ǰ��Ŀ������dll��ʱ���dllexport����
// ������Ŀû������LURENJIAMODELTOOL_API_EXPORTS����꣬������ĿҲ�ͻ���__declspec(dllimport)����
// ���������static�Ķ���������Ҫ���룬�����ȫ�ֵľͲ���Ҫ����
// https://blog.csdn.net/Repeaterbin/article/details/4269666 ��������ʾ��

#ifdef LURENJIAENGINE_API_EXPORTS
#define LURENJIAENGINE_API __declspec(dllexport)
#else
#define LURENJIAENGINE_API __declspec(dllimport)
#endif 