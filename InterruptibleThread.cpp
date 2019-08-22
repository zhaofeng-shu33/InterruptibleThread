#include <iostream>
#include <chrono>
#include <random>
#include <functional>
#include <mutex>
#include "InterruptibleThread.h"
using namespace std;
using std::function;

struct thread_interrupted {};

void interruption_point()
{
	if (this_thread_interrupt_flag.is_set()) {
		throw thread_interrupted();
	}
}
mutex mut;
void foo(promise<std::thread::id>* pro_ptr_, uniform_int_distribution<int>& u_)
{
    default_random_engine e_;
    bool tag = false;    
	while (true) {
		int res = u_(e_);
		if (res == 7) {
			if (!tag) {
				lock_guard<mutex> guard(mut);
				pro_ptr_->set_value(this_thread::get_id());
				tag = true;
			}
			break;
		}
        {
            lock_guard<mutex> guard(mut);            
            cout << this_thread::get_id() << " **** " << res << endl;
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
	uniform_int_distribution<int> u(0, 9);
	promise<std::thread::id> pro;
	future<std::thread::id> fut = pro.get_future();
	vector<InterruptibleThread> threads;
	cout << thread::hardware_concurrency() - 1 << endl;
	for (auto i = 0; i < thread::hardware_concurrency() - 1; i++) {
		threads.push_back(InterruptibleThread(foo, &pro, u));
	}
	if (fut.wait_for(chrono::milliseconds(1000)) == future_status::ready) {
		cout << "get it " << fut.get() << endl;
	}
	
	for (auto& t : threads) {
		t.interrupt();
		t.join();
	}
	return 0;
}
