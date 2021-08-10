#include "redisengine.h"
#include<set>

// 信息泵
// 按照定时发布消息
extern class MsgPumpConfig g_msgpump_conf;



// 消息引擎，负责消息代理连接，消息构建，消息发布

RedisEngine::RedisEngine( )
{    

    init();

    #ifdef DEB_STDOUT
    cout <<"Publish:"<<endl;
    cout <<"=========================\n";
    cout <<m_server_address<<endl;
    cout <<"user:"<<m_user<<"  password:" << m_pwd <<endl;
    cout <<"client tag:"<< m_cliendid << endl;
    #endif
}

RedisEngine::~RedisEngine()
{
}
    
 void RedisEngine::init()
{
    auto broker_map = g_msgpump_conf.getBrokerConf();

    string broke_ip = broker_map["ip"];
    string broke_port = broker_map["port"];
    string username = broker_map["user"];
    string pwd = broker_map["pwd"];   

    m_server_address = broke_ip;
    m_port = broke_port;
    m_user = username;
    m_pwd = pwd;

    
}

bool RedisEngine::connect()
{
    try{
        ConnectionOptions redis_conn_opt;
        redis_conn_opt.host = m_server_address;
        redis_conn_opt.port = stoul(m_port);   
        // 完成链接
        m_redis_client = make_shared<Redis>(redis_conn_opt);    
        }
    catch(const Error& e )
    {       
        return false;
    }   
     
    return true;
}

bool RedisEngine::disconnect()
{    
    return true;
}

bool RedisEngine::publish( )
{
    try{
        string payload = buildPayload();
        cout <<"\nPublish message ...{"<<payload<<"}"<<endl;;   
        m_redis_client->publish( m_topic, payload );
        
        return true;
    }
    
    catch(const Error& exc )
    {        
        return false;
    }

    return true;

}


string RedisEngine::buildPayload( )
{
    static unsigned long i_times = 0;
    string cur_tm = to_string((uintmax_t)time(NULL) );
    string result = m_topic + string(" Now times is:") + to_string(i_times++) + "..........";
    result+=cur_tm;            
    return result;
}
    