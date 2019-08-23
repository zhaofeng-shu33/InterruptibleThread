#ifndef INTERRUPTIBLETHREAD_H
#define INTERRUPTIBLETHREAD_H
#include <thread>
#include <functional>
#include <future>
#ifndef __cpp_lib_invoke
#define __cpp_lib_invoke
template<typename F, typename... Args>
auto invoke(F f, Args&&... args)
    -> decltype(std::ref(f)(std::forward<Args>(args)...))
{
    return std::ref(f)(std::forward<Args>(args)...);
}
#endif
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
    
    extern thread_local InterruptFlag this_thread_interrupt_flag;    
    
    void interruption_point();
    
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
                invoke(f, args...);
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

        void join();

        std::thread::id get_id();

        InterruptFlag* getInterruptFlag();

    private:
        InterruptFlag* _interrupt_flag;
    };

}
#endif // INTERRUPTIBLETHREAD_H
