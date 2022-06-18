#define _GEN_LOG_H_
#ifdef _GEN_LOG_H_
#include<memory>
#include<sstream>
#include<string>
#include<vector>
#include<iostream>
#include<fstream>
#include<algorithm>
#include<list>
#include<map>
#include<functional>

namespace gen
{
    class Logger;
    class Formatter;
    class LogLevel
    {
        public:
            enum Level
            {
                DEBUG,
                INFO,
                WARN,
                ERROR,
                FATAL    
            };
        static const char* toString(LogLevel::Level level);
    };
    
    class LogEvent
    {
        public:
            typedef std::shared_ptr<LogEvent> ptr;
            LogEvent(const std::string & msg);
            LogEvent(std::shared_ptr<Logger>,LogLevel::Level level,
            const char* file,
            uint32_t line,
            uint32_t elapse,
            uint32_t thread_id,
            uint32_t fiber_id,
            uint64_t time,
            const std::string & thread_name)
            :m_filename(file),
            m_line(line),
            m_elapse(elapse),
            thread_id(thread_id),
            fiber_id(fiber_id),
            time(time)
            {}
            std::string getMsg()const {return msg;}
            u_int32_t getLine()const {return m_line;}
            u_int32_t getElapse()const {return m_elapse;}
            const char * getLevel()const {return LogLevel::toString(m_level);}
           
            u_int64_t getThread()const {return thread_id;}
            time_t getTime()const {return time;}
            const char* getFilename()const {return m_filename;}
        private:
            const char * m_filename = nullptr;
            u_int32_t m_line = 0;
            u_int32_t m_elapse = 0;//程序启动到现在的毫秒数
            u_int64_t thread_id = -1;
            u_int64_t fiber_id = -1;
            time_t time;//时间戳
            LogLevel::Level m_level;
            std::string msg;
    };
    class Appender
    {
        public:
            typedef std::shared_ptr<Appender> ptr;
            Appender(const std::string & name,LogLevel::Level level): m_name(name), m_level(level){}
            virtual ~Appender() = 0;
            virtual void log(std::shared_ptr<Logger> logger,LogLevel::Level level, LogEvent::ptr event) = 0;
            void setFormatter(std::shared_ptr<Formatter> formatter){this->formater = formatter;}
            std::shared_ptr<Formatter> getFormatter() const {return formater;}
        protected:
            LogLevel::Level m_level;
            std::string m_name;
            std::shared_ptr<Formatter> formater;
    };
    class Logger : public std::enable_shared_from_this<Logger>
    {
        public:
            typedef std::shared_ptr<Logger> ptr;
            Logger(const std::string & name = "root");
            void log(LogLevel::Level level,LogEvent::ptr event);
            void addAppender(Appender::ptr appender);
            void delAppender(Appender::ptr appender);
            std::string  getName()const{return m_name;}
            LogLevel::Level getLevel()const {return m_level;}
            void setLevel(LogLevel::Level level){m_level = level;}
        private:
            std::string m_name;//日志的名称
            LogLevel::Level m_level;//满足日志级别的日志才会通过该日志器进行输出
            std::list<Appender::ptr> appenders;//输出到目的地的集合
            void debug(LogLevel::Level level,LogEvent::ptr event);
            void info(LogLevel::Level level,LogEvent::ptr event);
            void error(LogLevel::Level level,LogEvent::ptr event);
            void warn(LogLevel::Level level,LogEvent::ptr event);
            void fatal(LogLevel::Level level,LogEvent::ptr event);
            
    };
    class StdoutAppender : public Appender
    {
        public:
            typedef std::shared_ptr<StdoutAppender> ptr;
            ~StdoutAppender(){}
            StdoutAppender(const std::string & name = "root",LogLevel::Level level = LogLevel::INFO):Appender(name,level){}
            virtual void log(Logger::ptr logger,LogLevel::Level level,LogEvent::ptr event) override;
        private:

    };
    class FileAppender : public Appender
    {
        public:
            FileAppender(const std::string & filename,const std::string & name = "root",LogLevel::Level level = LogLevel::INFO):
            Appender(name,level),
            m_filename(filename)
            {
                reopen();
            }
            virtual void log(Logger::ptr logger,LogLevel::Level level,LogEvent::ptr event) override;
            bool reopen()
            {
                if(fs)
                {
                    fs.close();
                }
                fs.open(m_filename,std::ios_base::app);
                return !!fs;
            }
            ~FileAppender(){fs.close();}
        private:
            std::string m_filename;
            std::fstream fs;
    };
    class Formatter
    {
        public:
            typedef std::shared_ptr<Formatter> ptr;
            std::string format(typename Logger::ptr logger,LogEvent::ptr event);
            Formatter(const std::string & pattern);
        
            class FormatItem
            {
                public:
                    typedef std::shared_ptr<FormatItem> ptr;
                    virtual ~FormatItem(){}
                    virtual void format(Logger::ptr logger,std::ostream & os,LogEvent::ptr event) = 0;
                protected:
            };
        private:
            std::vector<FormatItem::ptr> m_items;
            std::string m_pattern;
            std::tuple<std::string,std::string,int> process(const std::string & str);
            void init();
    };
    
}
#endif