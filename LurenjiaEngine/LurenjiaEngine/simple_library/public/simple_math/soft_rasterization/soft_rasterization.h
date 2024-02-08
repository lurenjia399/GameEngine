// Copyright (C) RenZhai.2022.All Rights Reserved.
#include "simple_library/public/simple_library.h"
#include "simple_library/public/simple_library_macro.h"

#define VARIABLE(...)

namespace soft_rasterization
{
	////////////////////////////对象
	//万物皆为对象
	class SIMPLE_LIBRARY_API fobject
	{
		friend class fengine;
	public:
		fobject();

	public:
		virtual void build(float in_time) {}
		virtual void tick(float in_time) {}
		virtual void clear() {}

	public:
		template<class t>
		t *create_object()
		{
			gobject_array.push_back(new t());

			return dynamic_cast<t*>(gobject_array[gobject_array.size() - 1]);
		}

	private:
		static void gobject_array_init(float in_time);
		static void gobject_array_tick(float in_time);
		static void gobject_array_clear();

	private:
		static std::vector<fobject*> gobject_array;
	};

	//组件基类
	class SIMPLE_LIBRARY_API fcomponent : public fobject
	{
	public:
		fcomponent* parent;
		std::vector<fcomponent*> children;
	};

	//位移组件
	class SIMPLE_LIBRARY_API ftransform_component :public fcomponent
	{
	public:
		ftransform_component();

		virtual void build(float in_time) {}
		virtual void tick(float in_time);
		virtual void clear() {}

	public:
		virtual void set_rot(const frotator &in_rot);

		virtual frotator get_rot() const { return rotation; }

	public:
		fvector_3d position;
		frotator rotation;
		fvector_3d scale;

		//旋转有关
		fvector_3d forward_vector;
		fvector_3d right_vector;
		fvector_3d up_vector;

		//如果是摄像机 那么它是把世界点转为局部空间下。
		//如果是模型的worldmatrix 那么它将把局部点转到世界空间下
		fmatrix_4x4 matrix;
		fmatrix_4x4 viewProj_matrix;

	protected:
		frotator last_rotation;
	};

	//Mesh组件
	class SIMPLE_LIBRARY_API fmesh_component :public ftransform_component
	{
	public:
		std::vector<fvector_3d> vertex_data;
	};

	//能看到的基类
	class SIMPLE_LIBRARY_API factor :public fobject
	{
		VARIABLE(category = actor, visibleanywhere, blueprintreadonly, meta = (allowprivateaccess = "true"))
		ftransform_component* transform;//root component

	public:
		factor();
		virtual ~factor();

		virtual void set_rot(const frotator& in_rot);
		virtual frotator get_rot() const {return transform->get_rot();}

		FORCEINLINE ftransform_component* get_transform() { return transform; }
	};

	//摄像机
	class SIMPLE_LIBRARY_API fcamera :public factor
	{
	public:
	};

	//模型Actor
	class fmesh_actor :public factor
	{
		VARIABLE(category = actor, visibleanywhere, blueprintreadonly, meta = (allowprivateaccess = "true"))
		fmesh_component* mesh_component;

	public:
		fmesh_actor();
		~fmesh_actor();

		FORCEINLINE fmesh_component* get_mesh() { return mesh_component; }
	};

	//视口配置
	struct SIMPLE_LIBRARY_API fviewport_config
	{
		fviewport_config()
			:viewport_size(1920, 1080)
			, fov(0.5f * 3.1415926f)
			, aspect_ratio(viewport_size.x / viewport_size.y)
			, near_z(1.f)
			, far_z(1000.f)
		{
		}

		fvector_2d viewport_size;

		float fov;
		float aspect_ratio;
		float near_z;
		float far_z;
	};
	////////////////////////////要渲染的数据
	struct SIMPLE_LIBRARY_API frender_data_3d
	{
		std::vector<fvector_3d> vertex_data;
		std::vector<uint16_t> index_data;

		fmatrix_4x4 matrix;
	};

	struct SIMPLE_LIBRARY_API frender_data_2d
	{
		std::vector<fvector_2d> vertex_data;
		std::vector<uint16_t> index_data;
	};

	////////////////////////////引擎
	class SIMPLE_LIBRARY_API fengine
	{
	public:
		virtual void init(float in_time);
		virtual void tick(float in_time);
		virtual void exit();
	};

	class SIMPLE_LIBRARY_API frender_engine :public fengine
	{
		typedef fengine super;

	public:
		virtual void init(float in_time);
		virtual void tick(float in_time);
		virtual void exit();

	public:
		void build_input_path(const std::wstring& in_path);
		void build_draw_object(const std::vector<fmesh_actor*>& in_objs);
		void build_camera(const fvector_3d& in_position, const fviewport_config& in_config);
	
	public:
		void strat_update(float in_time, bool breset_bone = false);
		void draw(float in_time);
		void end_update(float in_time);

	protected:
		fcamera camera;
		fviewport_config viewport_config;

		std::vector<fmesh_actor*> draw_obj;

		//每一帧要绘制的数据
		std::vector<frender_data_3d> frame_render_data3;
		std::wstring wpath;

		//帧数
		int index;//
	};

	class SIMPLE_LIBRARY_API fengine_factory
	{
	public:
		template<class t>
		t* create_engine()
		{
			return new t();
		}
	};
}

/*
#include "simple_library/public/simple_math/soft_rasterization/soft_rasterization.h"

using namespace soft_rasterization;

int main()
{
	class ftest_mesh_actor :public soft_rasterization::fmesh_actor
	{
		typedef soft_rasterization::fmesh_actor super;
	public:
		virtual void tick(float in_time)
		{
			super::tick(in_time);

			frotator r = get_rot();

			//r.yaw += 10.f;
			r.pitch -= 5.f;
			//r.roll += 14.f;
			set_rot(r);

			//
			//// 位置
			//get_transform()->position.x -= 1.f;
			//get_transform()->position.y -= 1.f;
			//get_transform()->position.z -= 1.f;
			////// 旋转
			////get_transform()->rotation.yaw += 6.f;
			//// 缩放
			//get_transform()->scale.x -= 0.1f;
			//get_transform()->scale.y -= 0.4f;
			//get_transform()->scale.z -= 0.4f;
		}
	};

	fengine_factory factory;
	if (frender_engine* render_enigne = factory.create_engine<frender_engine>())
	{
		std::vector<fmesh_actor*> in_objs;
		{
			in_objs.push_back(new ftest_mesh_actor());
			fmesh_actor* in_actor = in_objs[in_objs.size() - 1];

			//三角形 面片
			in_actor->get_mesh()->vertex_data.push_back(fvector_3d(0.f, 0.f, 0.f));
			in_actor->get_mesh()->vertex_data.push_back(fvector_3d(0.f, 10.f, 10.f));
			in_actor->get_mesh()->vertex_data.push_back(fvector_3d(10.f, 0.f, 0.f));
		}

		//1.转路径
		char path_buff[1024] = { 0 };
		char path[] = "../Math/render/a_%i.bmp";
		get_full_path(path_buff, 1024, path);

		wchar_t dst_wchar_t[1024] = { 0 };
		char_to_wchar_t(dst_wchar_t, 1024, path_buff);

		fviewport_config config;
		render_enigne->build_draw_object(in_objs);
		render_enigne->build_camera(fvector_3d(1.f, 40.f, 1.f), config);
		render_enigne->build_input_path(dst_wchar_t);

		render_enigne->init(0.2f);

		while (1)
		{
			render_enigne->tick(0.03f);
		}

		render_enigne->exit();
	}

	return 0;
}
*/