#define __GEN_THREAD_H__
#ifdef __GEN_THREAD_H__
#include<thread>
#include<memory>
#include<functional>
#include<string>
#include<stdexcept>

#include<pthread.h>

namespace gen
{
    class Thread
    {
        public:
            typedef std::shared_ptr<Thread> ptr;
            Thread(std::function<void()> cb,const std::string & name);

            const std::string &getName() const {return m_name;}

            void join();
            static Thread * GetThis();
            static void SetName(const std::string & name);
            static const std::string & GetName();
            ~Thread();
        private:
            static void* run(void* arg);
            std::string m_name;
            Thread(const Thread & thread) = delete;
            Thread(const Thread && thread) = delete;
            Thread operator = (const Thread & t) = delete;
            pid_t m_id = -1 ;
            pthread_t m_thread;
            std::function<void()> m_cb;
    };
} // namespace gen
#endif
