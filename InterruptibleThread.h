#ifndef INTERRUPTIBLETHREAD_H
#define INTERRUPTIBLETHREAD_H
#include <thread>
#include <future>
namespace InterruptibleThread{
    struct thread_interrupted {};
    
    class InterruptFlag
    {
    public:
        inline void set() {
            _set = true;
        }

        inline bool is_set() {
            return _set;
        }

    private:
        bool _set;
    };
    
    thread_local InterruptFlag this_thread_interrupt_flag;    
    
    void interruption_point()
    {
        if (this_thread_interrupt_flag.is_set()) {
            throw thread_interrupted();
        }
    }
    

    class thread
    {
    public:
        std::thread _internal_thread;
        template<typename FunctionType, typename... Args>
        thread(FunctionType&& f, Args&&... args)
        {
            std::promise<InterruptFlag*> p;
            _internal_thread = std::thread([&f, &p, &args...]()
            {
                p.set_value(&this_thread_interrupt_flag);
                f(args...);
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

        std::thread::id get_id() {
            return std::this_thread::get_id();
        }

        InterruptFlag* getInterruptFlag() {
            return &this_thread_interrupt_flag;
        }

    private:
        InterruptFlag* _interrupt_flag;
    };
}
#endif // INTERRUPTIBLETHREAD_H