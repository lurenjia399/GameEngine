#include "simple_library/public/simple_math/math_utils.h"

namespace math_utils
{
    fvector_3d mul(const fvector_2d& in_3d, const fmatrix_3x3& in_matrix_3x3)
    {
        return mul(fvector_3d(in_3d.x, in_3d.y, 1.f), in_matrix_3x3);
    }

    fvector_3d mul(const fvector_3d& in_3d, const fmatrix_3x3& in_matrix_3x3)
    {
        return fvector_3d(
            in_3d.x * in_matrix_3x3.m11 + in_3d.y * in_matrix_3x3.m21 + in_3d.z * in_matrix_3x3.m31,
            in_3d.x * in_matrix_3x3.m12 + in_3d.y * in_matrix_3x3.m22 + in_3d.z * in_matrix_3x3.m32,
            in_3d.x * in_matrix_3x3.m13 + in_3d.y * in_matrix_3x3.m23 + in_3d.z * in_matrix_3x3.m33);
    }

    fvector_4d mul(const fvector_4d& in_3d, const fmatrix_4x4& in_matrix_3x3)
    {
        return fvector_4d(
            in_3d.x * in_matrix_3x3.m11 +
            in_3d.y * in_matrix_3x3.m21 +
            in_3d.z * in_matrix_3x3.m31 +
            in_3d.w * in_matrix_3x3.m41,

            in_3d.x * in_matrix_3x3.m12 +
            in_3d.y * in_matrix_3x3.m22 +
            in_3d.z * in_matrix_3x3.m32 +
            in_3d.w * in_matrix_3x3.m42,

            in_3d.x * in_matrix_3x3.m13 +
            in_3d.y * in_matrix_3x3.m23 +
            in_3d.z * in_matrix_3x3.m33 +
            in_3d.w * in_matrix_3x3.m43,

            in_3d.x * in_matrix_3x3.m14 +
            in_3d.y * in_matrix_3x3.m24 +
            in_3d.z * in_matrix_3x3.m34 +
            in_3d.w * in_matrix_3x3.m44);
    }

    float angle_to_radian(float angle)
    {
        return angle * 0.017453f;//angle * (π / 180）= radian
    }

    float radian_to_angle(float radian)
    {
        return radian * 57.29578f;//radian * (180/π）= angle
    }

    void rot_radian(float in_radian, fmatrix_3x3& in_world_matrix_3x3)
    {
        //旋转矩阵
        fmatrix_3x3 rot_matrix_3x3;

        //公式
        rot_matrix_3x3.m11 = cos(in_radian);
        rot_matrix_3x3.m12 = -sin(in_radian);
        rot_matrix_3x3.m21 = sin(in_radian);
        rot_matrix_3x3.m22 = cos(in_radian);

        //矩阵乘法
        in_world_matrix_3x3 = rot_matrix_3x3 * in_world_matrix_3x3;
    }

    void rot_angle(float in_angle, fmatrix_3x3& in_world_matrix_3x3)
    {
        float in_radian = in_angle * (3.1415926 / 180.0);

        rot_radian(in_radian, in_world_matrix_3x3);
    }

    void set_scale(const fvector_2d& in_scale, fmatrix_3x3& in_world_matrix_3x3)
    {
        in_world_matrix_3x3.m11 = in_scale.x;
        in_world_matrix_3x3.m22 = in_scale.y;
    }

    fmatrix_3x3 set_scale(const fvector_2d& in_scale)
    {
        fmatrix_3x3 in_world_matrix_3x3;
        set_scale(in_scale, in_world_matrix_3x3);

        return in_world_matrix_3x3;
    }

    bool scalar_neare_qual_float(float f1, float f2, float epsilon)
    {
        return fabsf(f1 - f2) <= epsilon;
    }

    bool scalar_neare_qual_int(int i1, int i2, int epsilon)
    {
        return abs(i1 - i2) <= epsilon;
    }

    fmatrix_4x4 matrix_perspective(
        float in_fov_radian,
        float aspect_ratio,
        float near_z, float far_z)
    {
        assert(near_z > 0.f && far_z > 0.f);
        assert(!scalar_neare_qual_float(in_fov_radian, 0.0f, 0.00001f * 2.f));
        assert(!scalar_neare_qual_float(aspect_ratio, 0.0f, 0.00001f));
        assert(!scalar_neare_qual_float(near_z, far_z, 0.00001f));

        //构建透视矩阵
        fmatrix_4x4 matrix_4x4;
        {
            float t = tan(in_fov_radian * 0.5f) * near_z;
            float b = -t;
            float r = aspect_ratio * t;
            float l = -r;

            float d = far_z - near_z;

            matrix_4x4.m11 = (2.f * near_z) / (r - l);
            matrix_4x4.m22 = (2.f * near_z) / (t - b);
            matrix_4x4.m33 = -((far_z + near_z) / d);
            matrix_4x4.m34 = -1.f;
            matrix_4x4.m43 = -(2 * near_z * far_z / d);
            matrix_4x4.m44 = 0.f;
        }

        return matrix_4x4;
    }

    fmatrix_4x4 matrix_look_at_target(const fvector_4d& in_view_pos, const fvector_4d& in_target_pos, const fvector_4d& in_view_up)
    {
        fvector_4d n = in_target_pos - in_view_pos;
        n.normalize();//单位化

        fvector_4d u = fvector_4d::cross_product(in_view_up, n);
        u.normalize();

        fvector_4d v = fvector_4d::cross_product(n, u);
        v.normalize();

        return fmatrix_4x4(
            u.x, v.x, n.x, 0.f,
            u.y, v.y, n.y, 0.f,
            u.z, v.z, n.z, 0.f,
            -fvector_4d::dot(u, in_view_pos),
            -fvector_4d::dot(v, in_view_pos),
            -fvector_4d::dot(n, in_view_pos), 1.f);
    }

    fmatrix_4x4 build_view_matrix(const fvector_4d& in_view_pos, const fmatrix_4x4& in_view_matrix)
    {
        fvector_4d u = fvector_4d(in_view_matrix.m11, in_view_matrix.m21, in_view_matrix.m31, 1.f);
        fvector_4d v = fvector_4d(in_view_matrix.m12, in_view_matrix.m22, in_view_matrix.m32, 1.f);
        fvector_4d n = fvector_4d(in_view_matrix.m13, in_view_matrix.m23, in_view_matrix.m33, 1.f);

        return fmatrix_4x4(
            u.x, v.x, n.x, 0.f,
            u.y, v.y, n.y, 0.f,
            u.z, v.z, n.z, 0.f,
            -fvector_4d::dot(u, in_view_pos),
            -fvector_4d::dot(v, in_view_pos),
            -fvector_4d::dot(n, in_view_pos), 1.f);
    }
    fmatrix_4x4 matrix_rotation_y(const float angle)
    {
        float radian = angle_to_radian(angle);

        return fmatrix_4x4(
            cos(radian), 0.f, sin(radian), 0.f,
            0.f, 1.f, 0.f, 0.f,
            -sin(radian), 0.f, cos(radian), 0.f,
            0.f, 0.f, 0.f, 1.f);
    }
    fmatrix_4x4 matrix_rotation_x(const float angle)
    {
        float radian = angle_to_radian(angle);

        return fmatrix_4x4(
            1.f, 0.f, 0.f, 0.f,
            0.f, cos(radian), -sin(radian), 0.f,
            0.f, sin(radian), cos(radian), 0.f,
            0.f, 0.f, 0.f, 1.f);
    }
    fmatrix_4x4 matrix_rotation_z(const float angle)
    {
        float radian = angle_to_radian(angle);

        return fmatrix_4x4(
            cos(radian), -sin(radian), 0.f, 0.f,
            sin(radian), cos(radian), 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            0.f, 0.f, 0.f, 1.f);
    }

    fmatrix_4x4 matrix_rotation_axis(const fvector_3d& axis, const float angle)
    {
        fvector_3d n = axis;
        n.normalize();

        float radian = angle_to_radian(angle);

        return fmatrix_4x4(
            n.x * n.x * (1.f - cos(radian)) + cos(radian), n.x * n.y * (1.f - cos(radian)) - n.z * sin(radian), n.x * n.z * (1.f - cos(radian)) + n.y * sin(radian), 0.f,
            n.x * n.y * (1.f - cos(radian)) + n.z * sin(radian), n.y * n.y * (1.f - cos(radian)) + cos(radian), n.y * n.z * (1.f - cos(radian)) - n.x * sin(radian), 0.f,
            n.x * n.z * (1.f - cos(radian)) - n.y * sin(radian), n.z * n.y * (1.f - cos(radian)) + n.x * sin(radian), n.z * n.z * (1.f - cos(radian)) + cos(radian), 0.f,
            0.f, 0.f, 0.f, 1.f);
    }

    void inertia_to_object(const fquat& in_quat, fmatrix_3x3& out_rotation_matrix)
    {
        out_rotation_matrix.m11 = 1.f - 2.f * (in_quat.y * in_quat.y + in_quat.z * in_quat.z);
        out_rotation_matrix.m12 = 2.f * (in_quat.x * in_quat.y + in_quat.w * in_quat.z);
        out_rotation_matrix.m13 = 2.f * (in_quat.x * in_quat.z - in_quat.w * in_quat.y);

        out_rotation_matrix.m21 = 2.f * (in_quat.x * in_quat.y - in_quat.w * in_quat.z);
        out_rotation_matrix.m22 = 1.f - 2.f * (in_quat.x * in_quat.x + in_quat.z * in_quat.z);
        out_rotation_matrix.m23 = 2.f * (in_quat.y * in_quat.z + in_quat.w * in_quat.x);

        out_rotation_matrix.m31 = 2.f * (in_quat.x * in_quat.z + in_quat.w * in_quat.y);
        out_rotation_matrix.m32 = 2.f * (in_quat.y * in_quat.z - in_quat.w * in_quat.x);
        out_rotation_matrix.m33 = 1.f - 2.f * (in_quat.x * in_quat.x + in_quat.y * in_quat.y);
    }

    void object_to_inertia(const fquat& in_quat, fmatrix_3x3& out_rotation_matrix)
    {
        inertia_to_object(in_quat, out_rotation_matrix);

        out_rotation_matrix.transpose();
    }

    fvector_3d inertia_to_object(const fvector_3d& in_vector, const fmatrix_3x3& in_rotation_matrix)
    {
        return mul(in_vector, in_rotation_matrix);
    }

    fvector_3d object_to_inertia(const fvector_3d& in_vector, const fmatrix_3x3& in_rotation_matrix)
    {
        fmatrix_3x3 matrix = in_rotation_matrix.to_transpose();

        return mul(in_vector, matrix);
    }

    void matrix_to_quat(const fmatrix_3x3& in_rotation_matrix, fquat& out_quat)
    {
        float value[4] = { 0 };

        value[0] = in_rotation_matrix.m11 + in_rotation_matrix.m22 + in_rotation_matrix.m33;//w
        value[1] = in_rotation_matrix.m11 - in_rotation_matrix.m22 - in_rotation_matrix.m33;//x
        value[2] = -in_rotation_matrix.m11 + in_rotation_matrix.m22 - in_rotation_matrix.m33;//y
        value[3] = -in_rotation_matrix.m11 - in_rotation_matrix.m22 + in_rotation_matrix.m33;//z

        float tmp_value = 0.f;

        int index = 0;
        for (int i = 0; i < 4; i++)
        {
            if (value[i] > tmp_value)
            {
                tmp_value = value[i];
                index = i;
            }
        }

        if (tmp_value != 0.f)
        {
            float value_sqrt = sqrt(tmp_value + 1.f) * 0.5f;
            float mult = 0.25f / value_sqrt;

            switch (index)
            {
            case 0://w
            {
                out_quat.w = value_sqrt;
                out_quat.x = (in_rotation_matrix.m23 - in_rotation_matrix.m32) * mult;
                out_quat.y = (in_rotation_matrix.m31 - in_rotation_matrix.m13) * mult;
                out_quat.z = (in_rotation_matrix.m12 - in_rotation_matrix.m21) * mult;
                break;
            }
            case 1://x
            {
                out_quat.w = (in_rotation_matrix.m23 - in_rotation_matrix.m32) * mult;
                out_quat.x = value_sqrt;
                out_quat.y = (in_rotation_matrix.m12 + in_rotation_matrix.m21) * mult;
                out_quat.z = (in_rotation_matrix.m31 - in_rotation_matrix.m13) * mult;
                break;
            }
            case 2://y
            {
                out_quat.w = (in_rotation_matrix.m31 - in_rotation_matrix.m13) * mult;
                out_quat.x = (in_rotation_matrix.m12 + in_rotation_matrix.m21) * mult;
                out_quat.y = value_sqrt;
                out_quat.z = (in_rotation_matrix.m23 + in_rotation_matrix.m32) * mult;
                break;
            }
            case 3://z
            {
                out_quat.w = (in_rotation_matrix.m12 - in_rotation_matrix.m21) * mult;
                out_quat.x = (in_rotation_matrix.m31 + in_rotation_matrix.m13) * mult;
                out_quat.y = (in_rotation_matrix.m23 + in_rotation_matrix.m32) * mult;
                out_quat.z = value_sqrt;
                break;
            }
            }
        }
    }

    fquat matrix_to_quat(const fmatrix_3x3& in_rotation_matrix)
    {
        fquat quat;

        matrix_to_quat(in_rotation_matrix, quat);

        return quat;
    }

    fquat pow(const fquat& in_q, float in_exponent)
    {
        fquat r;
        if (fabsf(in_q.w) > 0.9999f)
        {
            return in_q;
        }

        float angle = acos(in_q.w);

        float new_angle = angle * in_exponent;

        r.w = cos(new_angle);

        float in_value = sin(new_angle) / sin(angle);

        r.x = in_q.x * in_value;
        r.y = in_q.y * in_value;
        r.z = in_q.z * in_value;

        return r;
    }
}