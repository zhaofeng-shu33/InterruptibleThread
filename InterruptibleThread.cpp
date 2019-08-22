#include "InterruptibleThread.h"
namespace InterruptibleThread{
        
    thread_local InterruptFlag this_thread_interrupt_flag; 
    
    void interruption_point()
    {
        if (this_thread_interrupt_flag.is_set()) {
            throw thread_interrupted();
        }
    }
    


    void thread::join() {
        _internal_thread.join();
    }

    std::thread::id thread::get_id() {
        return std::this_thread::get_id();
    }

    InterruptFlag* thread::getInterruptFlag() {
        return &this_thread_interrupt_flag;
    }

}