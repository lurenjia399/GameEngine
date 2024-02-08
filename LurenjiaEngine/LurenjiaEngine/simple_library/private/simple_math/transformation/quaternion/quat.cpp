#include "simple_library/public/simple_math/transformation/quaternion/quat.h"
#include "simple_library/public/simple_math/transformation/vector/vector_3d.h"
#include "simple_library/public/simple_math/transformation/rotator/rotator.h"
#include "simple_library/public/simple_math/math_libray.hpp"
#include "simple_library/public/simple_math/math_utils.h"
#include <cmath>
#include <assert.h> 

fquat::fquat()
{
	*this = identity();
}

fquat::fquat(float in_x, float in_y, float in_z, float in_w)
	:x(in_x)
	,y(in_y)
	,z(in_z)
	,w(in_w)
{
}

fquat fquat::operator*(const fquat& q) const
{
	fquat result;

	result.w = q.w * w - q.x * x - q.y * y - q.z * z;
	result.x = q.x * w + q.w * x + q.y * z - q.z * y;
	result.y = q.y * w + q.w * y + q.z * x - q.x * z;
	result.z = q.z * w + q.w * z + q.x * y - q.y * x;

	return result;
}

fquat fquat::operator*=(const fquat& q)
{
	*this = *this * q;
	return *this;
}

fquat fquat::operator*(const float& q) const
{
	return fquat(x * q,y * q,z * q,w * q);
}

fquat fquat::operator*(const int& q) const
{
	return fquat(x * q, y * q, z * q, w * q);
}

fquat fquat::operator*=(const float& q)
{
	x *= q;
	y *= q;
	z *= q;
	w *= q;
	return *this;
}

fquat fquat::operator*=(const int& q)
{
	x *= q;
	y *= q;
	z *= q;
	w *= q;
	return *this;
}

fquat fquat::operator+(const fquat& q) const
{
	return fquat(x + q.x, y + q.y, z + q.z, w + q.w);
}

fquat fquat::operator-(const fquat& q) const
{
	return fquat(x - q.x, y - q.y, z - q.z, w - q.w);
}

fquat fquat::operator+=(const fquat& q)
{
	x += q.x;
	y += q.y;
	z += q.z;
	w += q.w;
	return *this;
}

fquat fquat::operator-=(const fquat& q)
{
	x -= q.x;
	y -= q.y;
	z -= q.z;
	w -= q.w;
	return *this;
}

float fquat::operator|(const fquat& q)const
{
	return w*q.w + x*q.x + y*q.y + z*q.z;
}

fquat fquat::log() const
{
	fquat result;
	result.w = 0.f;
	float scale = 1.f;
	if (fabsf(w) < 1.f)//cos(theta/2)
	{
		float angle = acos(w);
		float sin_angle = sin(angle);
		if (sin_angle >= 1.e-8f)
		{
			scale = angle / sin_angle;
		}
	}

	result.x = scale * x;
	result.y = scale * y;
	result.z = scale * z;

	return result;
}

fquat fquat::exp() const
{
	float angle = sqrt(x * x + y * y + z * z);
	float sin_angle = sin(angle);

	float scale = 1.f;

	fquat result;
	result.w = cos(angle);
	if (fabsf(sin_angle) >= 1.e-8f)
	{
		scale = sin_angle / angle;
	}

	result.x = scale * x;
	result.y = scale * y;
	result.z = scale * z;

	return result;
}

float fquat::size() const
{
	return sqrt(size_squared());
}

float fquat::size_squared() const
{
	return x * x + y * y + z * z + w * w;
}

float fquat::get_half_angle_radian() const
{
	return acos(w);
}

float fquat::get_angle_radian() const
{
	return get_half_angle_radian() * 2.f;
}

float fquat::get_angle() const
{
	return get_angle_radian() * (180.f / 3.1415926);
}

void fquat::normalize(float in_tolerance)
{
	const float square_sum = size_squared();

	if (square_sum >= in_tolerance)
	{
		float quat_size = sqrt(square_sum);

		x /= quat_size;
		y /= quat_size;
		z /= quat_size;
		w /= quat_size;
	}
	else
	{
		*this = identity();
	}	
}

bool fquat::is_normalized() const
{
	return abs((1.f - size_squared())) <= 0.01f;
}

void fquat::rotator_by_x(float theta)
{
	rotator_by_axis(theta, fvector_3d(1.f, 0.f, 0.f));
}

void fquat::rotator_by_y(float theta)
{
	rotator_by_axis(theta, fvector_3d(0.f, 1.f, 0.f));
}

void fquat::rotator_by_z(float theta)
{
	rotator_by_axis(theta, fvector_3d(0.f, 0.f, 1.f));
}

void fquat::rotator_by_axis(float theta, const fvector_3d& axis)
{
	assert(is_normalized());//至少单位化

	float helf_theta = theta * 0.5f;
	float sin_helf_theta = sin(helf_theta);

	//cos + sin v
	w = cos(helf_theta);
	x = axis.x * sin_helf_theta;
	y = axis.y * sin_helf_theta;
	z = axis.z * sin_helf_theta;
}

fquat fquat::inverse() const
{
	assert(is_normalized());//至少单位化

	return fquat(-x,-y,-z,w);
}

void fquat::inertia_to_object(const frotator& in_rot)
{
	feuler euler;
	in_rot.rotator_to_euler(euler);

	//半角
	euler /= 2.f;

	float sin_heading = sin(euler.heading);
	float sin_pitch = sin(euler.pitch);
	float sin_bank = sin(euler.bank);

	float cos_heading = cos(euler.heading);
	float cos_pitch = cos(euler.pitch);
	float cos_bank = cos(euler.bank);

	//套公式
	w = cos_heading * cos_pitch * cos_bank + sin_heading * sin_pitch * sin_bank;
	x = -cos_heading * sin_pitch * cos_bank - sin_heading * cos_pitch * sin_bank;
	y = cos_heading * sin_pitch * sin_bank - sin_heading * cos_pitch * cos_bank;
	z = sin_heading * sin_pitch * cos_bank - cos_heading * cos_pitch * sin_bank;
}

void fquat::object_to_inertia(const frotator& in_rot)
{
	feuler euler;
	in_rot.rotator_to_euler(euler);

	//半角
	euler /= 2.f;

	float sin_heading = sin(euler.heading);
	float sin_pitch = sin(euler.pitch);
	float sin_bank = sin(euler.bank);

	float cos_heading = cos(euler.heading);
	float cos_pitch = cos(euler.pitch);
	float cos_bank = cos(euler.bank);

	//套公式
	w = cos_heading * cos_pitch * cos_bank + sin_heading * sin_pitch * sin_bank;
	x = cos_heading * sin_pitch * cos_bank + sin_heading * cos_pitch * sin_bank;
	y = sin_heading * cos_pitch * cos_bank - cos_heading * sin_pitch * sin_bank;
	z = cos_heading * cos_pitch * sin_bank - sin_heading * sin_pitch * cos_bank;
}


void fquat::get_cos_sin(
	const frotator& in_rot,
	float& cos_h_roll,
	float& sin_h_roll,
	float& cos_h_pitch,
	float& sin_h_pitch,
	float& cos_h_yaw,
	float& sin_h_yaw)
{
	frotator tmp_rot;
	tmp_rot.pitch = math_utils::angle_to_radian(in_rot.pitch);
	tmp_rot.roll = math_utils::angle_to_radian(in_rot.roll);
	tmp_rot.yaw = math_utils::angle_to_radian(in_rot.yaw);

	cos_h_roll = cos(in_rot.roll * .5f);
	sin_h_roll = sin(in_rot.roll * .5f);
	cos_h_pitch = cos(in_rot.pitch * .5f);
	sin_h_pitch = sin(in_rot.pitch * .5f);
	cos_h_yaw = cos(in_rot.yaw * .5f);
	sin_h_yaw = sin(in_rot.yaw * .5f);
}

void fquat::object_to_inertia_v2(const frotator& in_rot)
{
	frotator tmp_rot;
	tmp_rot.pitch = math_utils::angle_to_radian(in_rot.pitch);
	tmp_rot.roll = math_utils::angle_to_radian(in_rot.roll);
	tmp_rot.yaw = math_utils::angle_to_radian(in_rot.yaw);

	float cos_h_roll = 0.f;
	float sin_h_roll = 0.f;
	float cos_h_pitch = 0.f;
	float sin_h_pitch = 0.f;
	float cos_h_yaw = 0.f;
	float sin_h_yaw = 0.f;

	get_cos_sin(tmp_rot, cos_h_roll, sin_h_roll, cos_h_pitch, sin_h_pitch, cos_h_yaw, sin_h_yaw);

	w = cos_h_roll * cos_h_pitch * cos_h_yaw + sin_h_roll * sin_h_pitch * sin_h_yaw;
	x = cos_h_roll * sin_h_pitch * cos_h_yaw + sin_h_roll * cos_h_pitch * sin_h_yaw;
	y = cos_h_roll * cos_h_pitch * sin_h_yaw - sin_h_roll * sin_h_pitch * cos_h_yaw;
	z = sin_h_roll * cos_h_pitch * cos_h_yaw - cos_h_roll * sin_h_pitch * sin_h_yaw;
}

void fquat::inertia_to_object_v2(const frotator& in_rot)
{
	float cos_h_roll = 0.f;
	float sin_h_roll = 0.f;
	float cos_h_pitch = 0.f;
	float sin_h_pitch = 0.f;
	float cos_h_yaw = 0.f;
	float sin_h_yaw = 0.f;

	get_cos_sin(in_rot, cos_h_roll, sin_h_roll, cos_h_pitch, sin_h_pitch, cos_h_yaw, sin_h_yaw);

	/// Cartesian coordinate System
	w = cos_h_roll * cos_h_pitch * cos_h_yaw + sin_h_roll * sin_h_pitch * sin_h_yaw;
	x = sin_h_roll * cos_h_pitch * cos_h_yaw - cos_h_roll * sin_h_pitch * sin_h_yaw;
	y = cos_h_roll * sin_h_pitch * cos_h_yaw + sin_h_roll * cos_h_pitch * sin_h_yaw;
	z = cos_h_roll * cos_h_pitch * sin_h_yaw - sin_h_roll * sin_h_pitch * cos_h_yaw;
}

frotator fquat::get_rot_by_object_to_inertia_v2()
{
	frotator rot;

	rot.roll = atan2(2 * (w * z + x * y), 1 - 2 * (z * z + x * x));
	rot.pitch = asin(math_libray::Clamp(2 * (w * x - y * z), -1.0f, 1.0f));
	rot.yaw = atan2(2 * (w * y + z * x), 1 - 2 * (x * x + y * y));

	rot.pitch = math_utils::radian_to_angle(rot.pitch);
	rot.roll = math_utils::radian_to_angle(rot.roll);
	rot.yaw = math_utils::radian_to_angle(rot.yaw);

	return rot;
}

frotator fquat::get_rot_by_inertia_to_object_v2()
{
	frotator rot;

	rot.roll = atan2(2 * (w * x + y * z), 1 - 2 * (x * x + y * y));
	rot.pitch = asin(2 * (w * y - z * x));
	rot.yaw = atan2(2 * (w * z + x * y), 1 - 2 * (y * y + z * z));

	rot.pitch = math_utils::radian_to_angle(rot.pitch);
	rot.roll = math_utils::radian_to_angle(rot.roll);
	rot.yaw = math_utils::radian_to_angle(rot.yaw);

	return rot;
}
fquat fquat::lerp(const fquat& in_q0, const fquat& in_q1, float in_t)
{
	//考虑双倍覆盖问题
	//q -q
	float bias = (in_q0 | in_q1) > 0.f ?1.f :-1.f;
	return (in_q0 * (bias * (1 - in_t))) + in_q1 * in_t;
}

fquat fquat::bilinear_lerp(
	const fquat& in_q00, const fquat& in_q10,
	const fquat& in_q01, const fquat& in_q11, 
	float frac_x, float frac_y)
{
	return lerp(
		lerp(in_q00, in_q10, frac_x),
		lerp(in_q01, in_q11, frac_y),
		frac_y);
}

fquat fquat::s_lerp(const fquat& in_q0, const fquat& in_q1, float in_t)
{
	if (in_t >= 1.f)
	{
		return in_q1;
	}
	else if (in_t <= 0.f)
	{
		return in_q0;
	}

	//双倍覆盖
	float q0oq1_cos = (in_q0 | in_q1);//cos
	q0oq1_cos = q0oq1_cos >= 0.f ? q0oq1_cos:-q0oq1_cos;

	assert(q0oq1_cos < 1.1f);

	//角度接近0.f的时候
	float scale0 = 0.f;
	float scale1 = 0.f;
	if (q0oq1_cos >= 0.99999f)
	{
		//线性Lerp
		scale0 = 1.f - in_t;
		scale1 = in_t;
	}
	else
	{
		//Slerp
		//sin
		float q0oq1_sin = sqrt(1.f - q0oq1_cos * q0oq1_cos);

		//角度(弧度)
		float theta = atan2(q0oq1_sin,q0oq1_cos);

		float q0oq1_inv_sin = 1.f / q0oq1_sin;

		//Slerp 公式
		scale0 = sin((1.f - in_t) * theta) * q0oq1_inv_sin;
		scale1 = sin(in_t * theta) * q0oq1_inv_sin;
	}

	//双倍覆盖
	scale1 = scale1 >= 0.f ? scale1 : -scale1;

	fquat result;
	result.x = scale0 * in_q0.x + scale1 * in_q1.x;
	result.y = scale0 * in_q0.y + scale1 * in_q1.y;
	result.z = scale0 * in_q0.z + scale1 * in_q1.z;
	result.w = scale0 * in_q0.w + scale1 * in_q1.w;

	return result;
}

fquat fquat::s_lerp_full_path(const fquat& in_q0, const fquat& in_q1, float in_t)
{
	float scale0 = 0.f;
	float scale1 = 0.f;

	float q0oq1_cos = (in_q0 | in_q1);//cos
	q0oq1_cos = math_libray::Clamp(q0oq1_cos,-1.f,1.f);
	float angle = asin(q0oq1_cos);//弧度

	if (fabsf(angle) < 1.e-4f)//极小值
	{
		return in_q0;
	}

	//Slerp
	float q0oq1_sin = sqrt(1.f - q0oq1_cos * q0oq1_cos);

	float q0oq1_inv_sin = 1.f / q0oq1_sin;

	//Slerp 公式
	scale0 = sin((1.f - in_t) * angle) * q0oq1_inv_sin;
	scale1 = sin(in_t * angle) * q0oq1_inv_sin;

	return in_q0 * scale0 + in_q1 * scale1;
}

fquat fquat::s_quad(const fquat& in_q0, const fquat& in_q1, const fquat& in_s0, const fquat& in_s1, float in_t)
{
	fquat q1 = s_lerp(in_q0, in_q1, in_t);
	fquat q2 = s_lerp_full_path(in_s0, in_s1, in_t);
	fquat result = s_lerp_full_path(q1, q2, 2.f * in_t * (1.f - in_t));
	result.normalize();

	return result;
}

fquat fquat::s_quad_full_path(const fquat& in_q0, const fquat& in_q1, const fquat& in_s0, const fquat& in_s1, float in_t)
{
	fquat q1 = s_lerp_full_path(in_q0, in_q1, in_t);
	fquat q2 = s_lerp_full_path(in_s0, in_s1, in_t);
	fquat result = s_lerp_full_path(q1, q2, 2.f * in_t * (1.f - in_t));
	result.normalize();

	return result;
}

fquat fquat::get_tangents(const fquat& in_prev_q, const fquat& in_q, const fquat& in_nest_q)
{
	fquat inv_p = in_q.inverse();

	fquat r1 = (inv_p * in_prev_q).log();
	fquat r2 = (inv_p * in_nest_q).log();

	fquat pre_exp = (r1 + r2) * -0.25f;

	return in_q * pre_exp.exp();
}

const fquat fquat::identity()
{
	return fquat(0.f, 0.f, 0.f, 1.f);
}
