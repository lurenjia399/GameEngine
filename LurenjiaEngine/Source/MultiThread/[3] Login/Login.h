#include <iostream>
#include <future>
#include <deque>
#include <mutex>
#include <thread>

/*
* 与 std::async等价的函数，其中运用了并发技术规约中的std::future
*/

namespace async
{
	// 返回的是future,当future.get的时候会一直等待，知道future中含有值。
	// future中的值，是由proise.set_value中设置进去的。
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
		t.detach();// 分离线程，失去对线程的所有权
		return res;
	}
}

// 后续函数的连锁调用
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
			return std::future<number>();//backend.authenticate_user(username, password); 这边是对账号密码的处理，看看是否能匹配
			}).then([](std::future<user_id> id){
					return std::future<number>(); //backend.request_current_info(id.get()); 这边请求该账号中的数据
				}).then([](std::future<user_data> info_to_display) {
					try {
						std::future<number>(); //update_display(info_to_display.get()); 这边就是显示账号信息
					}
					catch(std::exception& e){
						std::future<number>(e); //display_error(e); 抛出异常
					}
				});
	}
}

