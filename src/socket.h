#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>

#include<string>
#include<sstream>

namespace Gen
{
    /**
     * 封装linux的socket接口，创建服务器类
    */
    class Socket
    {
        public:
            Socket();
            virtual void init() = 0;
            virtual ~Socket() = 0;
            virtual int send() = 0;
            virtual int recv() = 0;
        private:
            int socketfd;

    };

    class TcpSocket : public Socket
    {
        public:
        private:
            
    };
}