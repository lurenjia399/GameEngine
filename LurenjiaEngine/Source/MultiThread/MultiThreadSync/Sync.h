#include <iostream>
#include <future>
#include <deque>
#include <mutex>
#include <thread>
#include<utility>

/*
* ���ַ�ʽ����std::future
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
	std::mutex m;//����������
	std::deque<std::packaged_task<void()>> tasks;//����task����
	bool gui_shutdown_message_received(){}//gui�رս���
	void get_and_process_gui_message(){}//gui��ȡĳ����Ϣ��Ȼ����
	void gui_thread()
	{
		while (!gui_shutdown_message_received())//gui����û�йر�
		{
			get_and_process_gui_message();//������
			std::packaged_task<void()> task;
			{
				std::lock_guard<std::mutex> lk(m);//����
				if(tasks.empty()) continue;
				task = std::move(tasks.front());
				tasks.pop_front();
			}//����
			task();//ִ�о����task����ķ���
		}
	}
	std::thread gui_bg_thread(gui_thread);// �������߳�
	
	template<typename Func>
	std::future<void> post_task_for_gui_thread(Func f)
	{
		std::packaged_task<void()> task(f);//ͨ����������Task
		std::future<void> result = task.get_future();//��ȡtask��future
		std::lock_guard<std::mutex> lk(m);//����
		tasks.push_back(std::move(task));//��task����dequeue��
		return result;//��future����
	}//����


	void main()
	{
		if (gui_bg_thread.joinable())
		{
			//gui_bg_thread.join();//���
			gui_bg_thread.detach();//����
		}
		//std::unique_ptr<int> ptr(new int(15));
		//std::unique_ptr<int> copy_ptr = std::move(ptr);
	}
}

namespace three
{
	void print_int(std::future<int>& fut) {
		int x = fut.get();                    // ��ȡ����״̬��ֵ.
		std::cout << "value: " << x << '\n';  // ��ӡ value: 10.
	}

	int main()
	{
		std::promise<int> prom;                    // ����һ�� std::promise<int> ����.
		std::future<int> fut = prom.get_future();  // �� future ����.
		std::thread t(print_int, std::ref(fut));   // �� future ��������һ���߳�t.
		prom.set_value(10);                        // ���ù���״̬��ֵ, �˴����߳�t����ͬ��.
		t.join();
		return 0;
	}
}
