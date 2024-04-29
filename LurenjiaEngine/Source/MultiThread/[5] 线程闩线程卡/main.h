#include <iostream>
#include <future>
#include <deque>
#include <mutex>
#include <thread>
#include <latch>
#include <barrier>

/*
* �� std::async�ȼ۵ĺ��������������˲���������Լ�е�std::future
*/

// �߳���
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
		std::latch done(thread_count);//�����߳��ţ��������߳�����
		my_data data[thread_count];
		std::vector<std::future<void>> threads;
		for (unsigned i = 0; i < thread_count; i++)
		{
			threads.push_back(std::async(std::launch::async, [&, i] {
				data[i] = make_data(i);
				done.count_down();//�߳��ż�һ����ʾ���߳��Ѿ��ߵ���
				//����������
				}));
			done.wait();//�����̵߳ȴ����ݴ�����
			process_data(data,thread_count);
		}
		
	}
}

// �߳̿�
namespace barrier
{
	struct data_block
	{

	};
	struct data_cource
	{
		data_block get_next_data_block(){}//��ȡ���ݿ�
		bool done(){return false; }
	};
	struct data_sink
	{
		void write_data(result_block){}//д������
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
		void set_chunk(unsigned, unsigned, bool){}//���ý����ÿ������
	};
	
	joining_thread joining_thread_func(std::function<void()>){}//����joining_thread�̵߳ķ���
	std::vector<data_chunk> divide_into_chunks(data_block, unsigned){}//�����ݿ�ֳɼ���
	bool process(data_chunk){return false;}//����ÿ������

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
			threads[i] = joining_thread_func([&, i]{//ѭ�������߳�
				while (!source.done())//����û�д������
				{
					if (!i)//ֻ��0���̻߳��߽�ȥ
					{
						// ���������ݷֳ�num_threads��
						data_block current_block = source.get_next_data_block();
						chunks = divide_into_chunks(current_block, num_threads);
					}
					sync.arrive_and_wait();//�����̵߳ȴ����ݻ��ֺã��ȴ������̵߳���
					result.set_chunk(i, num_threads, process(chunks[i]));// ÿ���̴߳�������
					sync.arrive_and_wait();//�����̵߳ȴ����ݴ���ã��ȴ������̵߳���
					if (!i)//ֻ��0���̻߳��߽�ȥ
					{
						// д������
						sink.write_data(std::move(result));
					}
				}
				});
		}
	}
}

