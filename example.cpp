#include <iostream>
#include <chrono>
#include <random>
#include <mutex>
#include "InterruptibleThread.h"
typedef InterruptibleThread::thread_interrupted thread_interrupted;
typedef InterruptibleThread::thread thread;
const auto& interruption_point = InterruptibleThread::interruption_point;
std::mutex mut;
void foo(std::promise<std::thread::id>* pro_ptr_, std::uniform_int_distribution<int>& u_)
{
    std::default_random_engine e_;
    bool tag = false;    
	while (true) {
		int res = u_(e_);
		if (res == 7) {
			if (!tag) {
				std::lock_guard<std::mutex> guard(mut);
				pro_ptr_->set_value(std::this_thread::get_id());
				tag = true;
			}
			break;
		}
        {
            std::lock_guard<std::mutex> guard(mut);            
            std::cout << std::this_thread::get_id() << " **** " << res << std::endl;
        }
		try {
			interruption_point();
		}
		catch (const thread_interrupted& interrupt) {
			break;
		}
	}
	while (true) {
		try {
			interruption_point();
		}
		catch (const thread_interrupted& interrupt) {
			break;
		}	
	}
}

int main() 
{
	std::uniform_int_distribution<int> u(0, 9);
	std::promise<std::thread::id> pro;
	std::future<std::thread::id> fut = pro.get_future();
	std::vector<thread> threads;
	std::cout << std::thread::hardware_concurrency() - 1 << std::endl;
	for (int i = 0; i < std::thread::hardware_concurrency() - 1; i++) {
		threads.push_back(thread(foo, &pro, u));
	}
	if (fut.wait_for(std::chrono::milliseconds(1000)) == std::future_status::ready) {
		std::cout << "get it " << fut.get() << std::endl;
	}
	
	for (thread& t : threads) {
		t.interrupt();
		t.join();
	}
	return 0;
}
