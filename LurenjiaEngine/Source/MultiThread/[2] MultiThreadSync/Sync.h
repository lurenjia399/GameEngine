#include <iostream>
#include <future>
#include <deque>
#include <mutex>
#include <thread>
#include<utility>

/*
* 三种方式创建std::future
* 1 std::async()
* 2 std::packaged_task<>
* 3 std::promise
*/

namespace one
{
	struct X
	{
		void foo(int, std::string const&) {}
		std::string bar(std::string const&) {}
	};
	X baz(X& inx)
	{
	}

	void main()
	{
		X x;
		std::future<X> f1 = std::async(std::launch::deferred, baz, std::ref(x));
		f1.wait();

	}
}

namespace two
{
	std::mutex m;//创建互斥量
	std::deque<std::packaged_task<void()>> tasks;//创建task队列
	bool gui_shutdown_message_received(){}//gui关闭界面
	void get_and_process_gui_message(){}//gui获取某个信息，然后处理
	void gui_thread()
	{
		while (!gui_shutdown_message_received())//gui界面没有关闭
		{
			get_and_process_gui_message();//做处理
			std::packaged_task<void()> task;
			{
				std::lock_guard<std::mutex> lk(m);//上锁
				if(tasks.empty()) continue;
				task = std::move(tasks.front());
				tasks.pop_front();
			}//解锁
			task();//执行具体的task里面的方法
		}
	}
	std::thread gui_bg_thread(gui_thread);// 创建新线程
	
	template<typename Func>
	std::future<void> post_task_for_gui_thread(Func f)
	{
		std::packaged_task<void()> task(f);//通过参数创建Task
		std::future<void> result = task.get_future();//获取task的future
		std::lock_guard<std::mutex> lk(m);//上锁
		tasks.push_back(std::move(task));//将task放入dequeue中
		return result;//将future传出
	}//解锁


	void main()
	{
		if (gui_bg_thread.joinable())
		{
			//gui_bg_thread.join();//汇合
			gui_bg_thread.detach();//分离
		}
		//std::unique_ptr<int> ptr(new int(15));
		//std::unique_ptr<int> copy_ptr = std::move(ptr);
	}
}

namespace three
{
	void print_int(std::future<int>& fut) {
		int x = fut.get();                    // 获取共享状态的值.
		std::cout << "value: " << x << '\n';  // 打印 value: 10.
	}

	int main()
	{
		std::promise<int> prom;                    // 生成一个 std::promise<int> 对象.
		std::future<int> fut = prom.get_future();  // 和 future 关联.
		std::thread t(print_int, std::ref(fut));   // 将 future 交给另外一个线程t.
		prom.set_value(10);                        // 设置共享状态的值, 此处和线程t保持同步.
		t.join();
		return 0;
	}
}
