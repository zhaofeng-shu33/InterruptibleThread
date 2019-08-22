#ifndef INTERRUPTIBLETHREAD_H
#define INTERRUPTIBLETHREAD_H
#include <thread>
#include <functional>
#include <future>
namespace InterruptibleThread{
    struct thread_interrupted {};
    
    class InterruptFlag
    {
    public:
        inline void set();

        inline bool is_set();

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
                auto f1 = std::bind(f, args...);
                f1();
            });
            _interrupt_flag = p.get_future().get();
        }

        inline void interrupt();

        void join();

        std::thread::id get_id();

        InterruptFlag* getInterruptFlag();

    private:
        InterruptFlag* _interrupt_flag;
    };
}
#endif // INTERRUPTIBLETHREAD_H