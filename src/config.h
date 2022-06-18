#define _GEN_CONFIG_H_
#ifdef _GEN_CONFIG_H_
#include<memory>
#include<sstream>
#include<string>
#include<stdexcept>
#include<iostream>
#include<map>

#include<boost/lexical_cast.hpp>
#include<yaml-cpp/yaml.h>
namespace gen
{
    class ConfigVarBase
    {
        protected:
            std::string m_name;
            std::string m_description;
        public:
            typedef std::shared_ptr<ConfigVarBase> ptr;
            ConfigVarBase(const std::string & name,const std::string & descirption = "")
            : m_name(name),
            m_description(descirption){}
            virtual ~ConfigVarBase(){}
            const std::string& getName()const {return m_name;}
            const std::string& getDescription()const {return m_description;}
            virtual std::string toString() = 0;
            virtual bool fromString(const std::string & str) = 0;
    };
    template<class T>
    class ConfigVar : public ConfigVarBase
    {
        public:
            typedef std::shared_ptr<ConfigVar> ptr;
            ConfigVar(const std::string & name,const T & value,const std::string & description = "")
            :ConfigVarBase(name,description),
            m_vaule(value){}
            std::string toString() override
            {
                try
                {
                    return boost::lexical_cast<std::string>(m_value);
                }
                catch(std::exception & e)
                {
                    std::errc<<e.what()<<"convert "<< typeid(m_value).name()<<" to string";
                    //todo : 用自己的日志库输出日志
                }
                return "";
            }
            bool fromString(const std::string & str) override
            {
                try
                {
                    
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << "convert string to"<< typeid(m_name).name()<<" error";
                }
                
            }
        private:
            T m_value;
            
    };
    class Config
    {
        public:
            typedef std::map<std::string,ConfigVarBase::ptr> ConfigVarMap;
        private:
            static ConfigVarMap m_datas;
        /**
         * 如果m_datas里没有对应的config，就创建一个
        */
        template<class T>
        static typename ConfigVar<T>::ptr Lookup(const std::string & name,const T & default_value,const std::string & description = "")
        {
            if(Lookup(name))
            {
                //打印日志，说明已经读取某个配置了。
            }
            else
            {
                if(name.find_first_not_of("abcdefghijklmnopqrstuvwxyz._0123456789")!=std::string::npos)
                {
                    //配置变量名非法，打日志，抛出异常
                    throw std::invalid_argument(name);
                }
                typename ConfigVar<T>::ptr tmp(new ConfigVar<T>(name,default_value,description));
                m_datas[name] = tmp;
            }
            return m_datas[name];
        }
        template<class T>
        static typename ConfigVar<T>::ptr Lookup(const std::string & name)
        {
            auto it = m_datas.find(name);
            if(it==m_datas.end())
                return nullptr;
            return std::dynamic_pointer_cast<ConfigVar<T> >(it->second);
        }
    };
}
#endif