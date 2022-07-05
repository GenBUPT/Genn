#define __GEN_THREAD_H__
#ifdef __GEN_THREAD_H__
#include<thread>
#include<memory>
#include<functional>
#include<string>
#include<stdexcept>
#include<pthread.h>
#include<cstdio>
namespace gen
{
    class Thread
    {
        typedef uint32_t tid_t;
        public:
            typedef std::shared_ptr<Thread> ptr;
            Thread(std::function<void()> cb,const std::string & name);

            const std::string &getName() const {return m_name;}
            const pid_t getId() const {return m_id;}
            void setName(const std::string & name){m_name = name;}
            void join();
            static Thread * GetThis();
            static void SetName(const std::string & name);
            static const std::string & GetName();
            static void* run(void* arg);
            ~Thread();
        private:
            
            std::string m_name;
            Thread(const Thread & thread) = delete;
            Thread(const Thread && thread) = delete;
            Thread operator = (const Thread & t) = delete;
            tid_t m_id = -1 ;
            pthread_t m_thread;
            std::function<void()> m_cb;
    };
} // namespace gen
#endif
