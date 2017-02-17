#ifndef INTERRUPTIBLETHREAD_H
#define INTERRUPTIBLETHREAD_H

#include <thread>
#include <future>
#include <iostream>
#include <chrono>
#include <random>
#include <mutex>

using namespace std;

mutex mut;
condition_variable cv;
int tag;

class InterruptFlag
{
public:
	inline void set() {
		lock_guard<mutex> guard(_mtx);
		_set = true;
	}

	inline bool is_set() {
		lock_guard<mutex> guard(_mtx);
		return _set;
	}

private:
	mutex _mtx;
	bool _set;
};

thread_local InterruptFlag this_thread_interrupt_flag;

class InterruptibleThread
{
public:
	default_random_engine e_;
	uniform_int_distribution<int> u_;
	thread _internal_thread;
	promise<decltype(this_thread::get_id())>* pro_ptr_;
	template<typename FunctionType>
	InterruptibleThread(FunctionType f, promise<decltype(this_thread::get_id())>* pro_ptr, uniform_int_distribution<int> u): 
	pro_ptr_(pro_ptr), u_(u){
		promise<InterruptFlag*> p;
		_internal_thread = thread([f, &p, this]()
		{
			p.set_value(&this_thread_interrupt_flag);
			f(*this);
		});
		_interrupt_flag = p.get_future().get();
	}

	inline void interrupt()
	{
		if (_interrupt_flag != nullptr)
		{
			_interrupt_flag->set();
		}
	}

	void join() {
		_internal_thread.join();
	}

	thread::id get_id() {
		return this_thread::get_id();
	}

	InterruptFlag* getInterruptFlag() {
		return &this_thread_interrupt_flag;
	}

private:
	InterruptFlag* _interrupt_flag;
};

#endif // INTERRUPTIBLETHREAD_H