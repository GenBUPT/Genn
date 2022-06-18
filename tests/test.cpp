#include"src/log.h"

int main()
{
    gen::Logger::ptr logger(new gen::Logger);
    gen::FileAppender::ptr file_appender(new gen::FileAppender("./log.txt"));
    gen::StdoutAppender::ptr stdout_appender(new gen::StdoutAppender());
    gen::Formatter::ptr fmt(new gen::Formatter("%d %p %c %T %l"));
    file_appender->setFormatter(fmt);
    stdout_appender->setFormatter(fmt);
    logger->addAppender(stdout_appender);
    gen::LogEvent::ptr event(new gen::LogEvent(logger,gen::LogLevel::INFO,"test.cpp",32,124234,4,5,23124,"filetest1"));
    logger->log(gen::LogLevel::INFO,event);
    return 0;
}