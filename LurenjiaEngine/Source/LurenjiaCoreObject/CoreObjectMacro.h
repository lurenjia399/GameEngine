#pragma once

// ��������Ŀ�����ж��� LURENJIACOREOBJECT_API_EXPORTS ����꣬��ǰ��Ŀ������dll��ʱ���dllexport����
// ������Ŀû������ LURENJIACOREOBJECT_API_EXPORTS ����꣬������ĿҲ�ͻ���__declspec(dllimport)����
// ���������static�Ķ���������Ҫ���룬�����ȫ�ֵľͲ���Ҫ����
// https://blog.csdn.net/Repeaterbin/article/details/4269666 ��������ʾ��

#ifdef LURENJIACOREOBJECT_API_EXPORTS
#define LURENJIACOREOBJECT_API __declspec(dllexport)
#else
#define LURENJIACOREOBJECT_API __declspec(dllimport)
#endif 