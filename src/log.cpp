#include"log.h"

namespace gen
{
    Appender::~Appender(){}
    const char* LogLevel::toString(LogLevel::Level level)
    {
        switch (level)
        {
        #define XX(LEVEL) case LogLevel::LEVEL :\
            return #LEVEL;
        XX(DEBUG)
        XX(INFO)
        XX(WARN)
        XX(FATAL)
        #undef XX
        default:
            return "UNKNOW LEVEL";
        }
        return "UNKNOW LEVEL";
    }
    Logger::Logger(const std::string & name):m_name(name)
    {}
    void Logger::log(LogLevel::Level level,LogEvent::ptr event)
    {
        auto self = shared_from_this();
        if(m_level<=level)
        {
          for(auto & i : appenders)
          {
              i->log(self,level,event);
          }
        }
    }
    void Logger::addAppender(Appender::ptr appender)
    {
        if(std::find(appenders.begin(),appenders.end(),appender)==appenders.end())
        {
            appenders.push_back(appender);
        }
    }
    void Logger::delAppender(Appender::ptr appender)
    {
        for(auto i = appenders.begin();i!=appenders.end();i++)
        {
            if(*i == appender)
            {
                appenders.erase(i);
                break;
            }
        }
    }
    void Logger::debug(LogLevel::Level level,LogEvent::ptr event)
    {
        Logger::log(LogLevel::DEBUG,event);
    }
    void Logger::info(LogLevel::Level level,LogEvent::ptr event)
    {
        Logger::log(LogLevel::INFO,event);
    }
    void Logger::warn(LogLevel::Level level,LogEvent::ptr event)
    {
        Logger::log(LogLevel::WARN,event);
    }
    void Logger::error(LogLevel::Level level,LogEvent::ptr event)
    {
        Logger::log(LogLevel::ERROR,event);
    }
    void Logger::fatal(LogLevel::Level level,LogEvent::ptr event)
    {
        Logger::log(LogLevel::FATAL,event);
    }

    void StdoutAppender::log(Logger::ptr logger, LogLevel::Level level,LogEvent::ptr event)
    {
        if(level>=m_level)
        {
            std::cout<<formater->format(logger,event);
        }
    }
    void FileAppender::log(Logger::ptr logger, LogLevel::Level level,LogEvent::ptr event)
    {
        if(level>=m_level)
        {
            fs<<formater->format(logger,event);
        }
    }
    /**
     * 解析pattern
     * %xxx %xxxx{xxxxx} %%
     * 出现这之外的情况就将这一段视为参数错误
    */
   std::tuple<std::string,std::string,int> Formatter::process(const std::string& str)
   {
        std::string nstr;
        std::string pattern;
        int stat = 0;
        
        for (size_t i = 1; i < str.size();++i)
        {
            if (stat == 0)
            {
                if (str[i] != '{')
                    nstr.push_back(str[i]);
                else stat = 1;
            }
            else if (stat == 1)
            {
                if (str[i] != '}')
                {
                    pattern.push_back(str[i]);
                }
                else stat = 2;
            }
        }
        if (stat == 0)
        {
            if(nstr.empty()) return std::make_tuple("error", "format error : " + str, 4);
            return std::make_tuple(nstr, pattern, 2);
        }
        if (stat == 2)
        {
            if(nstr.empty())return std::make_tuple("error", "format error : " + str, 4);
            return std::make_tuple(nstr, pattern, 3);
        }
        return std::make_tuple("error", "format error : "+str, 4);
   }
    class StringFormat : public Formatter::FormatItem
    {
        public:
            StringFormat(const std::string &fmt):m_fmt(fmt){}
            void format(Logger::ptr logger,std::ostream & os,LogEvent::ptr event)
            {
                os<<m_fmt;
            }
        private:
            std::string m_fmt;
    };
    class LineNumberFormat : public Formatter::FormatItem
    {
        public:
            LineNumberFormat(const std::string & fmt){}
             void format(Logger::ptr logger,std::ostream &os,LogEvent::ptr event) override
            {
                os<<event->getLine();
            }
    };
    class FileNameFormat : public Formatter::FormatItem
    {
        public:
            FileNameFormat(const std::string &fmt){}
             void format(Logger::ptr logger,std::ostream &os,LogEvent::ptr event) override
            {
                os<<event->getFilename();
            }
    };
    class DateTimeFormat : public Formatter::FormatItem
    {
        public:
            DateTimeFormat(const std::string & fmt = "%Y-%m-%d %H:%M:%S"):m_fmt(fmt){}
             void format(Logger::ptr logger,std::ostream & os ,LogEvent::ptr event)override
            {
                if(m_fmt.empty())
                {
                    m_fmt = "%Y-%m-%d %H:%M:%S";
                }
                struct tm tm;
                time_t time = event->getTime();
                localtime_r(&time,&tm);
                char buf[64];
                strftime(buf,sizeof(buf),m_fmt.c_str(),&tm);
                os<<buf;
            }
        private:
            std::string m_fmt;

    };
    class LineFormat : public Formatter::FormatItem
    {
        public:
            LineFormat(const std::string & fmt){}
            void format(Logger::ptr logger,std::ostream & os,LogEvent::ptr event)
            {
                    os<<event->getLine();
            }
    };
    class NewLineFormat : public Formatter::FormatItem
    {
        public:
            NewLineFormat(const std::string & fmt){}
            void format(Logger::ptr logger,std::ostream & os,LogEvent::ptr event) override
            {
                os<<"\n";
            }
    };
    class ThreadFormat :public Formatter::FormatItem
    {
        public:
            ThreadFormat(const std::string & fmt){}
            void format(Logger::ptr logger,std::ostream & os,LogEvent::ptr event) override
            {
                os<<event->getThread();
            }
    };
    class LogNameFormat : public Formatter::FormatItem
    {
        public:
        LogNameFormat(const std::string & fmt){}
         void format(Logger::ptr logger, std::ostream & os,LogEvent::ptr event)override
        {
            os<<logger->getName();
        }
    };
    class ElaspFormat : public Formatter::FormatItem
    {
        public:
        ElaspFormat(const std::string & fmt){}
        void format(Logger::ptr logger,std::ostream & os,LogEvent::ptr event) override
        {
            os<<event->getElapse();
        }
    };
    class LevelFormat : public Formatter::FormatItem
    {
        public :
            LevelFormat(const std::string & fmt){}
             void format(Logger::ptr logger, std::ostream & os,LogEvent::ptr event) override
            {
                os<<event->getLevel();
            }
    };
    class MessageFormat : public Formatter::FormatItem
    {
        public:
            MessageFormat(const std::string & fmt){}
            void format(Logger::ptr logger,std::ostream &os,LogEvent::ptr event) override
            {
                os<<event->getMsg();
            }
    };

    class TableFormat : public Formatter::FormatItem
    {
        public:
            TableFormat(const std::string & fmt){}
            void format(Logger::ptr logger,std::ostream & os,LogEvent::ptr event) override
            {
                os<<"\t";
            }
    };

    Formatter::Formatter(const std::string & pattern) : m_pattern(pattern)
    {
        init();
    }
    std::string  Formatter::format(Logger::ptr logger,LogEvent::ptr event)
    {
        std::stringstream ss;
        for(auto &i : m_items)
        {
            i->format(logger,ss,event);
        }
        return ss.str();
    }
    void Formatter::init()
    {
        std::vector<std::tuple<std::string, std::string, int> > vec;
        size_t pos = 0;
        std::string nstr;
        while (pos < m_pattern.size())
        {
            
            if (m_pattern[pos] == '%')
            {
                if (pos < m_pattern.size() - 1 && m_pattern[pos + 1] == '%')
                {
                    nstr.push_back(m_pattern[pos+1]);
                    ++pos;
                }
                else
                {
                    if(!nstr.empty())
                    vec.push_back(std::make_tuple(nstr, "", 1));
                    nstr.clear();
                    size_t start = pos;
                    ++pos;
                    while (pos < m_pattern.size() && m_pattern[pos] != '%' && m_pattern[pos]!=' ')
                    {
                        ++pos;
                    } 
                    vec.push_back(process(m_pattern.substr(start, pos - start)));
                }
            }
            else
            {
                nstr.push_back(m_pattern[pos]);
                ++pos;
            }
        }
        if (!nstr.empty())
        {
            vec.push_back(std::make_tuple(nstr, "", 1));
        }
        static std::map<std::string,std::function<FormatItem::ptr(std::string &str)> >funs = 
        {
            #define XX(str,fun) \
            {#str,[](const std::string & fmt){return FormatItem::ptr(new fun(fmt));}}

            XX(m,MessageFormat),
            XX(p,LevelFormat),
            XX(r,ElaspFormat),
            XX(c,LogNameFormat),
            XX(t,ThreadFormat),
            XX(n,NewLineFormat),
            XX(d,DateTimeFormat),
            XX(f,FileNameFormat),
            XX(string,StringFormat),
            XX(l,LineFormat),
            XX(T,TableFormat)
            #undef XX
        };
        for(auto i : vec)
        {
            #define XX(t)\
            std::get<t>(i)
            if(XX(2)==2||XX(2)==3)
                m_items.push_back(funs[std::get<0>(i)](std::get<1>(i)));
            if(XX(2)==1)
                m_items.push_back(funs["string"](XX(0)));
        #undef XX 
                   
        }
        /**
         * %m message
         * %p level
         * %r 启动后的时间
         * %c 日志名称
         * %p 线程id
         * %n 回车换行
         * %d 时间
         * %f 文件名
         * %l 行号
         * 
        */
        
    }
   
   
}