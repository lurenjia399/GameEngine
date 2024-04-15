#include <iostream>
#include <future>
#include <deque>
#include <mutex>
#include <thread>

/*
* �� std::async�ȼ۵ĺ��������������˲���������Լ�е�std::future
*/

namespace async
{
	// ���ص���future,��future.get��ʱ���һֱ�ȴ���֪��future�к���ֵ��
	// future�е�ֵ������proise.set_value�����ý�ȥ�ġ�
	template<typename Func>
	std::future<decltype(std::declval<Func>()())> spawn_async(Func& func)
	{
		std::promise<decltype(std::declval<Func>()())> p;
		auto res = p.get_future();

		std::thread t(
			[p = std::move(p), f = std::decay_t<Func>(func)]() -> mutable{
			try {
				p.set_value_at_thread_exit(f());
			}
			catch (...){
				p.set_exception_at_thread_exit(std::current_exception());
			}
		});
		t.detach();// �����̣߳�ʧȥ���̵߳�����Ȩ
		return res;
	}
}

// ������������������
namespace login
{
	struct number
	{

	};
	struct user_id : public number
	{

	};
	struct user_data : public number
	{

	};
	std::future<void> process_login(std::string const& username, std::string const& password)
	{
		return async::spawn_async([=]()->std::future<number>{
			return std::future<number>();//backend.authenticate_user(username, password); ����Ƕ��˺�����Ĵ��������Ƿ���ƥ��
			}).then([](std::future<user_id> id){
					return std::future<number>(); //backend.request_current_info(id.get()); ���������˺��е�����
				}).then([](std::future<user_data> info_to_display) {
					try {
						std::future<number>(); //update_display(info_to_display.get()); ��߾�����ʾ�˺���Ϣ
					}
					catch(std::exception& e){
						std::future<number>(e); //display_error(e); �׳��쳣
					}
				});
	}
}

