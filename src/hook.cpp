#include"hook.hpp"

namespace gen
{
    bool is_hook_enable()
    {
        
    }
}
typedef unsigned int (*sleep_fun)(unsigned int seconds);
extern sleep_fun sleep_f;

typedef int (*usleep_fun)(useconds_t usec);
extern usleep_fun usleep_f;