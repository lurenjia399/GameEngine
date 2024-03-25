#pragma once
#include <vector>

#include "../simple_library_macro.h"
//向量
#include "transformation/vector/vector_2d.h"
#include "transformation/vector/vector_3d.h"
#include "transformation/vector/vector_4d.h"

//矩阵
#include "transformation/matrix/matrix_2x2.h"
#include "transformation/matrix/matrix_3x3.h"
#include "transformation/matrix/matrix_4x4.h"

//四元数
#include "transformation/quaternion/quat.h"

using namespace std;
namespace math_utils
{
	//矩阵乘法
	fvector_3d SIMPLE_LIBRARY_API mul(const fvector_2d &in_3d, const fmatrix_3x3& in_matrix_3x3);
	fvector_3d SIMPLE_LIBRARY_API mul(const fvector_3d &in_3d, const fmatrix_3x3& in_matrix_3x3);
	fvector_4d SIMPLE_LIBRARY_API mul(const fvector_4d &in_3d, const fmatrix_4x4& in_matrix_3x3);

	float SIMPLE_LIBRARY_API angle_to_radian(float angle);
	float SIMPLE_LIBRARY_API radian_to_angle(float radian);

	//矩阵旋转
	void SIMPLE_LIBRARY_API rot_radian(float in_radian, fmatrix_3x3& in_world_matrix_3x3);
	void SIMPLE_LIBRARY_API rot_angle(float in_angle, fmatrix_3x3& in_world_matrix_3x3);

	//矩阵缩放
	void SIMPLE_LIBRARY_API set_scale(const fvector_2d& in_scale, fmatrix_3x3& in_world_matrix_3x3);
	fmatrix_3x3 SIMPLE_LIBRARY_API set_scale(const fvector_2d& in_scale);

	//判定bool是不是在一定范围内
	bool SIMPLE_LIBRARY_API scalar_neare_qual_float(float f1, float f2, float epsilon);
	bool SIMPLE_LIBRARY_API scalar_neare_qual_int(int i1, int i2, int epsilon);

	fmatrix_4x4 SIMPLE_LIBRARY_API matrix_perspective(float in_fov_radian, float aspect_ratio, float near_z, float far_z);

	//看向目标
	fmatrix_4x4 SIMPLE_LIBRARY_API matrix_look_at_target(const fvector_4d &in_view_pos, const fvector_4d& in_target_pos, const fvector_4d& in_view_up);

	//构建透视矩阵
	fmatrix_4x4 SIMPLE_LIBRARY_API build_view_matrix(const fvector_4d& in_view_pos, const fmatrix_4x4& in_view_matrix);

	//旋转
	fmatrix_4x4 SIMPLE_LIBRARY_API matrix_rotation_y(const float angle);
	fmatrix_4x4 SIMPLE_LIBRARY_API matrix_rotation_x(const float angle);
	fmatrix_4x4 SIMPLE_LIBRARY_API matrix_rotation_z(const float angle);

	//任意轴旋转
	fmatrix_4x4 SIMPLE_LIBRARY_API matrix_rotation_axis(const fvector_3d &axis,const float angle);

	//四元数转矩阵
	//惯性->物体
	void SIMPLE_LIBRARY_API inertia_to_object(const fquat &in_quat, fmatrix_3x3& out_rotation_matrix);

	//物体->惯性
	void SIMPLE_LIBRARY_API object_to_inertia(const fquat &in_quat, fmatrix_3x3& out_rotation_matrix);

	//向量
	//惯性->物体 
	//默认的 in_rotation_matrix 是 惯->物体
	fvector_3d SIMPLE_LIBRARY_API inertia_to_object(const fvector_3d& in_vector,const fmatrix_3x3& in_rotation_matrix);

	//物体->惯性
	fvector_3d SIMPLE_LIBRARY_API object_to_inertia(const fvector_3d& in_vector, const fmatrix_3x3& in_rotation_matrix);

	//旋转矩阵->四元数
	void SIMPLE_LIBRARY_API matrix_to_quat(const fmatrix_3x3& in_rotation_matrix, fquat& out_quat);

	//旋转矩阵->四元数
	fquat SIMPLE_LIBRARY_API matrix_to_quat(const fmatrix_3x3& in_rotation_matrix);

	//四元数求幂
	fquat SIMPLE_LIBRARY_API pow(const fquat &in_q,float in_exponent);
}