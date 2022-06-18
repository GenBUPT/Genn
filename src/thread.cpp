#include"thread.h"
namespace gen
{
    static thread_local Thread* t_thread = nullptr;
    static thread_local std::string t_name = "UNKNOW";
    Thread * Thread::GetThis()
    {
        return t_thread;
    }
    const std::string & Thread::GetName() 
    {
        return t_name;
    }
    void Thread::SetName(const std::string & name)
    {
        if(t_thread) t_thread->SetName(name);
        t_name = name;
    }
    Thread::Thread(std::function<void()>cb,const std::string & name)
    {
        if(name.empty())m_name = "UNKNOW";
        int rt = pthread_create(&m_thread,nullptr,&Thread::run,this);
        if(rt)
        {
            throw std::logic_error("error thread create fail");
        }
    }
    void* Thread::run(void * arg)
    {
        
    }
    void Thread::join()
    {
        if(m_thread) 
        {
            int rt = pthread_join(m_thread,nullptr);
            if(rt)
            {
                throw std::logic_error("error join fail");
            }
        }
    }
    Thread::~Thread()
    {
        if(m_thread) pthread_detach(m_thread);
    }
}