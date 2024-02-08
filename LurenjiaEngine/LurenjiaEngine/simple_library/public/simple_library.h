#pragma once
// Copyright (C) RenZhai.2022.All Rights Reserved.

//该c库的详细源码讲解在AboutCG 《手把手入门硬核c语言》视频工程里面：
//https://www.aboutcg.org/courseDetails/902/introduce
//希望学习其他方面技术 比如做UE4游戏, 可以访问下列网址：
//https://zhuanlan.zhihu.com/p/60117613
//
//bibi可以看到各个人宅系列教程介绍：
//https://space.bilibili.com/29544409
//
//个人博客网站
//http://renzhai.net
//
//关于本套案例的详细操作 :
//文字版本(详细)：
//https://zhuanlan.zhihu.com/p/144558934
//视频版本：
//https://www.bilibili.com/video/BV1x5411s7s3

#ifdef CLOSE_DISABLE_DEBUG_INFOR
#pragma warning(disable:4267)//“参数”：从“xx”转换为“xx”，可能会丢失数据
#pragma warning(disable:4311)// 类型转换：指针从“您的ptr*”截断为“DWORD”
#pragma warning(disable:4244)// 从“xx”转换为“xx”，可能会丢失数据
#pragma warning(disable:4309)// 常数值截断
#pragma warning(disable:4305)//“参数”：从“int”截断为“u\u short”
#pragma warning(disable:4133)// function”：不兼容的类型-从“char[260]”到“LPWSTR”
#pragma warning(disable:4090)//“正在初始化”：不同的“const”限定符
#pragma warning(disable:4013)//“XXX”未定义；假设外部返回int
#pragma warning(disable:4251)// 以后开启当前状态检查dll情况
#pragma warning(disable:4717)// 在所有控制路径上递归，函数将导致运行时堆栈溢出
#pragma warning(disable:4099)// 首先使用“struct”看到的类型名称现在使用“class”看到
#pragma warning(disable:4018)// ：“<”：有符号/无符号不匹配
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

//第三方库
//xml
#include "../third_party_library/tinyxml/tinystr.h"
#include "../third_party_library/tinyxml/tinyxml.h"
