#include <iostream>
#include <future>
#include <deque>
#include <mutex>
#include <thread>
#include <latch>
#include <barrier>

/*
* 与 std::async等价的函数，其中运用了并发技术规约中的std::future
*/

// 线程闩
namespace latch
{
	struct my_data
	{

	};
	my_data make_data(int ){}
	void process_data(my_data*, unsigned){}
	void foo()
	{
		unsigned const thread_count = 5;
		std::latch done(thread_count);//创建线程闩，参数是线程数量
		my_data data[thread_count];
		std::vector<std::future<void>> threads;
		for (unsigned i = 0; i < thread_count; i++)
		{
			threads.push_back(std::async(std::launch::async, [&, i] {
				data[i] = make_data(i);
				done.count_down();//线程闩减一，表示此线程已经走到了
				//做其他操作
				}));
			done.wait();//所有线程等待数据创建好
			process_data(data,thread_count);
		}
		
	}
}

// 线程卡
namespace barrier
{
	struct data_block
	{

	};
	struct data_cource
	{
		data_block get_next_data_block(){}//获取数据块
		bool done(){return false; }
	};
	struct data_sink
	{
		void write_data(result_block){}//写入数据
	};
	struct joining_thread
	{
		void operator() (std::function<joining_thread()>)
		{
			
		}
	};
	struct data_chunk
	{

	};
	struct result_block
	{
		void set_chunk(unsigned, unsigned, bool){}//设置结果里每个数据
	};
	
	joining_thread joining_thread_func(std::function<void()>){}//创建joining_thread线程的方法
	std::vector<data_chunk> divide_into_chunks(data_block, unsigned){}//将数据块分成几份
	bool process(data_chunk){return false;}//处理每份数据

	void process_data(data_cource& source, data_sink& sink)
	{
		const unsigned concurrency = std::thread::hardware_concurrency();//
		const unsigned num_threads = (concurrency > 0)? concurrency:2;
		std::barrier sync(num_threads);
		std::vector<joining_thread> threads(num_threads);
		std::vector<data_chunk> chunks;
		result_block result;

		for (unsigned i = 0; i < num_threads; i++)
		{
			threads[i] = joining_thread_func([&, i]{//循环创建线程
				while (!source.done())//数据没有处理完成
				{
					if (!i)//只有0号线程会走进去
					{
						// 将整个数据分成num_threads块
						data_block current_block = source.get_next_data_block();
						chunks = divide_into_chunks(current_block, num_threads);
					}
					sync.arrive_and_wait();//所有线程等待数据划分好，等待所有线程到达
					result.set_chunk(i, num_threads, process(chunks[i]));// 每个线程处理数据
					sync.arrive_and_wait();//所有线程等待数据处理好，等待所有线程到达
					if (!i)//只有0号线程会走进去
					{
						// 写入数据
						sink.write_data(std::move(result));
					}
				}
				});
		}
	}
}

