#include<pthread.h>

#include<unistd.h>
#include<stdlib.h>
#include<syscall.h>

#include<string>
namespace gen
{
    pid_t GetThreadId();
    u_int32_t GetFiberId();
}