#pragma once
// Copyright (C) RenZhai.2022.All Rights Reserved.

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

#ifdef CLOSE_DISABLE_DEBUG_INFOR
#pragma warning(disable:4267)//�����������ӡ�xx��ת��Ϊ��xx�������ܻᶪʧ����
#pragma warning(disable:4311)// ����ת����ָ��ӡ�����ptr*���ض�Ϊ��DWORD��
#pragma warning(disable:4244)// �ӡ�xx��ת��Ϊ��xx�������ܻᶪʧ����
#pragma warning(disable:4309)// ����ֵ�ض�
#pragma warning(disable:4305)//�����������ӡ�int���ض�Ϊ��u\u short��
#pragma warning(disable:4133)// function���������ݵ�����-�ӡ�char[260]������LPWSTR��
#pragma warning(disable:4090)//�����ڳ�ʼ��������ͬ�ġ�const���޶���
#pragma warning(disable:4013)//��XXX��δ���壻�����ⲿ����int
#pragma warning(disable:4251)// �Ժ�����ǰ״̬���dll���
#pragma warning(disable:4717)// �����п���·���ϵݹ飬��������������ʱ��ջ���
#pragma warning(disable:4099)// ����ʹ�á�struct��������������������ʹ�á�class������
#pragma warning(disable:4018)// ����<�����з���/�޷��Ų�ƥ��
#pragma warning(disable:4477)
#pragma warning(disable:4244)//'argument': conversion from 'float' to 'int', possible loss of data
#endif // DEBUG

//core minimal
#include "simple_core_minimal/simple_c_guid/simple_guid.h"
#include "simple_core_minimal/simple_c_path/simple_path.h"
#include "simple_core_minimal/simple_c_windows/simple_c_windows_setting.h"
#include "simple_core_minimal/simple_c_windows/simple_c_windows_register.h"
#include "simple_core_minimal/simple_c_helper_file/simple_file_helper.h"
#include "simple_core_minimal/simple_c_time/simple_c_time.h"

//core
#include "simple_core_minimal/simple_c_core/simple_c_string_algorithm/string_algorithm.h"
#include "simple_core_minimal/simple_c_core/simple_c_array/simple_c_array.h"


//application 
#include "simple_c_log/simple_c_log.h"

//Delegate
#include "simple_delegate/simple_delegate.h"

//net
//#include "simple_channel/simple_net_drive.h"

//math
#include "simple_math/simple_core_math.h"

//image
#include "simple_image/simple_image.h"

//��������
//xml
#include "../third_party_library/tinyxml/tinystr.h"
#include "../third_party_library/tinyxml/tinyxml.h"
