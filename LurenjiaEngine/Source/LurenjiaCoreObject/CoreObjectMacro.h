#pragma once

// 我们在项目属性中定义 LURENJIACOREOBJECT_API_EXPORTS 这个宏，当前项目在生成dll的时候会dllexport导出
// 其他项目没有声明 LURENJIACOREOBJECT_API_EXPORTS 这个宏，其他项目也就会走__declspec(dllimport)导入
// 如果导出了static的东西，就需要导入，如果是全局的就不需要导入
// https://blog.csdn.net/Repeaterbin/article/details/4269666 贴个文章示例

#ifdef LURENJIACOREOBJECT_API_EXPORTS
#define LURENJIACOREOBJECT_API __declspec(dllexport)
#else
#define LURENJIACOREOBJECT_API __declspec(dllimport)
#endif 