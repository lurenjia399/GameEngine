#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <condition_variable>

template<typename T>
class threadsafe_queue
{
private:
	mutable std::mutex mut;//互斥必须用mutable修饰，针对const对象，允许其数据成员发生变动
	std::queue<T> data_queue;
	std::condition_variable data_cond;
public:
	threadsafe_queue() {}
	threadsafe_queue(const threadsafe_queue& other)
	{
		std::lock_guard<std::mutex> lk(other.mut);
		data_queue = other.data_queue;
	}
	threadsafe_queue& operator=(const threadsafe_queue&) = delete;

public:
	void push(T newvalue)
	{
		{
			std::lock_guard<std::mutex> lk(mut);// 拿到互斥并上锁
			data_queue.push(newvalue);//push数据
		}
		data_cond.notify_one();//通知push完成，（在通知的时候mut还未解锁）
	}
	bool try_pop(T& value)
	{
		std::lock_guard<std::mutex> lk(mut);
		if (data_queue.empty()) return false;
		value = data_queue.front();
		data_queue.pop();
		return true;
	}
	std::shared_ptr<T> try_pop()
	{
		std::lock_guard<std::mutex> lk(mut);
		if (data_queue.empty()) return std::shared_ptr<T>();
		std::shared_ptr<T> result = std::make_shared<T>(data_queue.front());
		data_queue.pop();
		return result;
	}
	void wait_and_pop(T& value)
	{
		std::unique_lock<std::mutex> lk(mut);
		data_cond.wait(lk, [this] {return this->data_queue.empty(); });//表达式false：解锁lk并等待。表达式true：上锁lk并返回wait()
		value = data_queue.front();
		data_queue.pop();
	}
	std::shared_ptr<T> wait_and_pop()
	{
		std::unique_lock<std::mutex> lk(mut);
		data_cond.wait(lk, [this] {return this->data_queue.empty(); });
		std::shared_ptr<T> result = std::make_shared<T>(data_queue.front());
		data_queue.pop();
		return result;
	}
	bool empty() const
	{
		std::lock_guard<std::mutex> lk(mut);
		return data_queue.empty();
	}
};