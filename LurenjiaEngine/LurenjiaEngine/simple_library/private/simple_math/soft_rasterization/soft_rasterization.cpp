#include "simple_library/public/simple_math/soft_rasterization/soft_rasterization.h"

namespace soft_rasterization
{
	std::vector<fobject*> fobject::gobject_array;

	factor::factor()
	{
		transform = create_object<ftransform_component>();
	}

	factor::~factor()
	{
		
	}

	void factor::set_rot(const frotator& in_rot)
	{
		transform->set_rot(in_rot);
	}

	fmesh_actor::fmesh_actor()
	{
		mesh_component = create_object<fmesh_component>();

		//注册子父级关系
		mesh_component->parent = get_transform();
		get_transform()->children.push_back(mesh_component);
	}

	fmesh_actor::~fmesh_actor()
	{
		
	}

	void frender_engine::init(float in_time)
	{
		super::init(in_time);
	}

	void frender_engine::tick(float in_time)
	{
		super::tick(in_time);

		//I 准备帧缓存数据
		strat_update(in_time);

		//II 渲染帧缓存数据
		draw(in_time);

		//III 清除帧缓存数据
		end_update(in_time);
	}

	void frender_engine::exit()
	{
		super::exit();
	}

	void frender_engine::build_input_path(const std::wstring& in_path)
	{
		wpath = in_path;
	}

	void frender_engine::build_draw_object(const std::vector<fmesh_actor*>& in_objs)
	{
		draw_obj = in_objs;
	}

	void frender_engine::build_camera(const fvector_3d& in_position, const fviewport_config& in_config)
	{
		viewport_config = in_config;
		camera.get_transform()->position = in_position;

		//构建view matrix
		fvector_4d up(0.f, 1.f, 0.f, 1.f);
		camera.get_transform()->matrix = math_utils::matrix_look_at_target(
			fvector_4d(
				camera.get_transform()->position.x,
				camera.get_transform()->position.y,
				camera.get_transform()->position.z,
				1.f), fvector_4d(0.f), up);

		//构建Proj_matrix
		fmatrix_4x4 Proj_matrix = math_utils::matrix_perspective(
			in_config.fov,//90
			in_config.aspect_ratio,
			in_config.near_z,
			in_config.far_z);

		//构建 viewProj_matrix
		Proj_matrix.transpose();
		camera.get_transform()->viewProj_matrix =
			camera.get_transform()->matrix * Proj_matrix;
	}

	void frender_engine::strat_update(float in_time, bool breset_bone)
	{
		frame_render_data3.clear();
		
		//注册帧缓存数据
		for (auto& tmp : draw_obj)
		{
			if (breset_bone)
			{
				frame_render_data3.push_back(frender_data_3d());
				frender_data_3d& data_3d = frame_render_data3[frame_render_data3.size() - 1];

				//I拿到模型数据
				data_3d.vertex_data = tmp->get_mesh()->vertex_data;

				//II world matrix
				//构建旋转
				fvector_3d forward_vector = tmp->get_mesh()->forward_vector;
				fvector_3d right_vector = tmp->get_mesh()->right_vector;
				fvector_3d up_vector = tmp->get_mesh()->up_vector;

				fvector_3d position = tmp->get_mesh()->position;
				fvector_3d scale = tmp->get_mesh()->scale;

				//UVN矩阵
				//矫正
				fvector_3d dot_position;
				dot_position.x = fvector_3d::dot(position, right_vector);
				dot_position.y = fvector_3d::dot(position, up_vector);
				dot_position.z = fvector_3d::dot(position, forward_vector);

				data_3d.matrix = {
				right_vector.x * scale.x,up_vector.x * scale.y,forward_vector.x * scale.z,0.f,
				right_vector.y * scale.x,up_vector.y * scale.y,forward_vector.y * scale.z,0.f,
				right_vector.z * scale.x,up_vector.z * scale.y,forward_vector.z * scale.z,0.f,
				dot_position.x,dot_position.y,dot_position.z,				1.f
				};

				//保存
				tmp->get_transform()->matrix = data_3d.matrix;
			}
			else
			{
				frame_render_data3.push_back(frender_data_3d());
				frender_data_3d& data_3d = frame_render_data3[frame_render_data3.size() - 1];
				data_3d.matrix = tmp->get_transform()->matrix;
			}
		}
	}

	bool is_inside_triangles(
		const std::vector<primitives::ftriangle>& in_triangles,
		int x, int y, fvector_4d& in_color)
	{
		for (auto& tmp : in_triangles)
		{
			if (primitives::is_inside_triangle(tmp, x, y))
			{
				in_color = tmp.Color;

				return true;
			}
		}

		return false;
	}

	//绘制帧缓存数据
	void frender_engine::draw(float in_time)
	{
		//相对坐标转世界
		vector<fvector_4d> world_pos;
		for (auto& tmp : frame_render_data3)
		{
			for (size_t i = 0; i < tmp.vertex_data.size(); i += 3)
			{
				{
					fvector_3d v = tmp.vertex_data[i];
					fvector_4d mesh_pos_world = math_utils::mul(fvector_4d(v.x, v.y, v.z, 1.f), tmp.matrix);
					mesh_pos_world.w = 1.f;
					world_pos.push_back(mesh_pos_world);
				}

				{
					fvector_3d v = tmp.vertex_data[i + 1];
					fvector_4d mesh_pos_world = math_utils::mul(fvector_4d(v.x, v.y, v.z, 1.f), tmp.matrix);
					mesh_pos_world.w = 1.f;
					world_pos.push_back(mesh_pos_world);
				}

				{
					fvector_3d v = tmp.vertex_data[i + 2];
					fvector_4d mesh_pos_world = math_utils::mul(fvector_4d(v.x, v.y, v.z, 1.f), tmp.matrix);
					mesh_pos_world.w = 1.f;
					world_pos.push_back(mesh_pos_world);
				}
			}
		}

		//转到裁剪空间
		vector<fvector_4d> clip_space;
		for (size_t i = 0; i < world_pos.size(); i += 3)
		{
			{
				fvector_4d point = world_pos[i];
				fvector_4d cs_point = math_utils::mul(point, camera.get_transform()->viewProj_matrix);

				clip_space.push_back(cs_point);
			}

			{
				fvector_4d point = world_pos[i + 1];
				fvector_4d cs_point = math_utils::mul(point, camera.get_transform()->viewProj_matrix);

				clip_space.push_back(cs_point);
			}

			{
				fvector_4d point = world_pos[i + 2];
				fvector_4d cs_point = math_utils::mul(point, camera.get_transform()->viewProj_matrix);

				clip_space.push_back(cs_point);
			}
		}

		//齐次除法 映射到NDC空间
		vector<fvector_4d> ndc_space;
		for (size_t i = 0; i < clip_space.size(); i += 3)
		{
			{
				fvector_4d ndc;
				ndc.x = clip_space[i].x / clip_space[i].w;
				ndc.y = clip_space[i].y / clip_space[i].w;
				ndc.z = clip_space[i].z / clip_space[i].w;

				ndc_space.push_back(ndc);
			}

			{
				fvector_4d ndc;
				ndc.x = clip_space[i + 1].x / clip_space[i + 1].w;
				ndc.y = clip_space[i + 1].y / clip_space[i + 1].w;
				ndc.z = clip_space[i + 1].z / clip_space[i + 1].w;

				ndc_space.push_back(ndc);
			}

			{
				fvector_4d ndc;
				ndc.x = clip_space[i + 2].x / clip_space[i + 2].w;
				ndc.y = clip_space[i + 2].y / clip_space[i + 2].w;
				ndc.z = clip_space[i + 2].z / clip_space[i + 2].w;

				ndc_space.push_back(ndc);
			}
		}

		frender_data_2d render_data2;
		//NDC映射到实际像素
		for (size_t i = 0; i < ndc_space.size(); i += 3)
		{
			render_data2.vertex_data.push_back(fvector_2d(
				(ndc_space[i].x * 0.5f + 0.5f) * viewport_config.viewport_size.x,//映射到实际屏幕空间
				(ndc_space[i].y * 0.5f + 0.5f) * viewport_config.viewport_size.y));//映射到实际屏幕空间

			render_data2.vertex_data.push_back(fvector_2d(
				(ndc_space[i + 1].x * 0.5f + 0.5f) * viewport_config.viewport_size.x,//映射到实际屏幕空间
				(ndc_space[i + 1].y * 0.5f + 0.5f) * viewport_config.viewport_size.y));//映射到实际屏幕空间

			render_data2.vertex_data.push_back(fvector_2d(
				(ndc_space[i + 2].x * 0.5f + 0.5f) * viewport_config.viewport_size.x,//映射到实际屏幕空间
				(ndc_space[i + 2].y * 0.5f + 0.5f) * viewport_config.viewport_size.y));//映射到实际屏幕空间
		}

		//2.拿到buff
		std::vector<unsigned char> data;
		SimpleImage::FImageInfo image_info;
		image_info.Height = (int)viewport_config.viewport_size.y;
		image_info.Width = (int)viewport_config.viewport_size.x;
		image_info.Channel = SimpleImage::EImageChannel::Channel_24Bit;
		image_info.ImageType = SimpleImage::EImageType::Bmp;
		SimpleImage::GetImageDataExcludeHeaderInfor(image_info, data);

		//3.准备三角形	
		std::vector<primitives::ftriangle> triangles;
		for (int i = 0; i < (int)render_data2.vertex_data.size(); i += 3)
		{
			triangles.push_back(primitives::ftriangle());
			primitives::ftriangle& in_triangle = triangles[triangles.size() - 1];

			in_triangle.point_1 = render_data2.vertex_data[i];
			in_triangle.point_2 = render_data2.vertex_data[i + 1];
			in_triangle.point_3 = render_data2.vertex_data[i + 2];
			in_triangle.Color.x = 32;
			in_triangle.Color.y = 100;
			in_triangle.Color.z = 255;
		}

		//4.光栅化 合并阶段
		for (int i = 0; i < image_info.Height; i++)
		{
			for (int j = 0; j < image_info.Width * (int)image_info.Channel; j += 3)
			{
				//row 行数
				int number_rows = i * image_info.Width * image_info.Channel;

				//像素的位置
				int x = j / image_info.Channel;
				int y = i;

				fvector_4d my_color;
				if (is_inside_triangles(triangles, x, y, my_color))
				{
					//BGR bmp
					data[number_rows + j] = my_color.x;			//B
					data[number_rows + j + 1] = my_color.y;		//G
					data[number_rows + j + 2] = my_color.z;		//R
				}
				else
				{
					//BGR bmp
					data[number_rows + j] = 0;		//B
					data[number_rows + j + 1] = 0;		//G
					data[number_rows + j + 2] = 0;		//R
				}
			}
		}

		//输出阶段
		wchar_t path[1024] = { 0 };
		if (!wpath.empty())
		{
			wget_printf(path, wpath.c_str(), index++);
		}
		else
		{
			wchar_t f[] = L"../Math/a_%i.bmp";
			wget_printf(path, f, index++);
		}

		bool bok = SimpleImage::SaveImageToDisk(
			image_info,
			path,
			data);

		open_url_w(path);
	}

	void frender_engine::end_update(float in_time)
	{
		//清除帧缓存数据
		frame_render_data3.clear();
	}

	fobject::fobject()
	{
		gobject_array.push_back(this);
	}

	void fobject::gobject_array_init(float in_time)
	{
		for (auto &tmp : gobject_array)
		{
			tmp->build(in_time);
		}
	}

	void fobject::gobject_array_tick(float in_time)
	{
		for (auto& tmp : gobject_array)
		{
			tmp->tick(in_time);
		}
	}

	void fobject::gobject_array_clear()
	{
		//通知上层逻辑 我要清除了
		for (auto& tmp : gobject_array)
		{
			tmp->clear();
		}

		//真正清除
		for (auto& tmp : gobject_array)
		{
			delete tmp;
			tmp = nullptr;
		}

		//再清除容器
		gobject_array.clear();
	}

	void fengine::init(float in_time)
	{
		fobject::gobject_array_init(in_time);
	}

	void fengine::tick(float in_time)
	{
		fobject::gobject_array_tick(in_time);
	}

	void fengine::exit()
	{
		fobject::gobject_array_clear();
	}

	ftransform_component::ftransform_component()
		:scale(1.f)
		, forward_vector(0.f, 0.f, 1.f)
		, right_vector(1.f, 0.f, 0.f)
		, up_vector(0.f, 1.f, 0.f)
	{

	}

	void ftransform_component::tick(float in_time)
	{
		for (auto &tmp : children)
		{
			if (ftransform_component *in_component = dynamic_cast<ftransform_component*>(tmp))
			{
				in_component->position = position;
				in_component->rotation = rotation;
				in_component->scale = scale;

				in_component->forward_vector = forward_vector;
				in_component->up_vector = up_vector;
				in_component->right_vector = right_vector;

				in_component->viewProj_matrix = viewProj_matrix;
				in_component->matrix = matrix;
			}
		}
	}

	void ftransform_component::set_rot(const frotator& in_rot)
	{
		//保存一下
		rotation = in_rot;

		//新的方法
		//将原先保存的矩阵转为欧拉角
		//frotator last_rotator;
		//fmatrix_3x3 last_matrix_3x3(matrix);
		//last_rotation.object_to_inertia(quat);

		frotator new_rotator =  rotation - last_rotation;

		printf("  rotation(yaw=%f.roll=%f.path=%f)\n", rotation.yaw, rotation.roll, rotation.pitch);
		printf("last_rotator(yaw=%f.roll=%f.path=%f)\n", last_rotation.yaw, last_rotation.roll, last_rotation.pitch);
		printf("------------------\n");

		//构建为旋转矩阵
		fmatrix_3x3 rot_matrix_3x3;
		//rot_matrix_3x3.object_to_inertia(in_rot); //旋转roll会出现问题
		fquat q;
		q.object_to_inertia(new_rotator);

		math_utils::object_to_inertia(q, rot_matrix_3x3);

		//构建新的for right up
		forward_vector = math_utils::mul(forward_vector, rot_matrix_3x3);
		right_vector = math_utils::mul(right_vector, rot_matrix_3x3);
		up_vector = math_utils::mul(up_vector, rot_matrix_3x3);

		last_rotation = rotation;
	}
}