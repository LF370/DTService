#include"redislisten.h"
#include<thread>
#include<chrono>
#include"msgpump.h"

// 信息泵
// 按照定时发布消息
extern class MsgPumpConfig g_msgpump_conf;



// 消息引擎，负责消息代理连接，消息构建，消息发布


RedisListener::RedisListener( MsgPump* _p_pump)
{    
    m_msg_pump = _p_pump;
    

    init();

    #ifdef DEB_STDOUT
    cout <<"Publish:"<<endl;
    cout <<"=========================\n";
    cout <<m_server_address<<endl;
    cout <<"user:"<<m_user<<"  password:" << m_pwd <<endl;
    cout <<"client tag:"<< m_cliendid << endl;
    #endif
}

RedisListener::~RedisListener()
{
}
    
void RedisListener::init()
{
    auto broker_map = g_msgpump_conf.getBrokerConf(2);

    string broke_ip = broker_map["ip"];
    string broke_port = broker_map["port"];
    string username = broker_map["user"];
    string pwd = broker_map["pwd"];   

    m_server_address = broke_ip;
    m_port = broke_port;
    m_user = username;
    m_pwd = pwd;

    
}

bool RedisListener::connect()
{
    try{
        ConnectionOptions redis_conn_opt;
        redis_conn_opt.host = m_server_address;
        redis_conn_opt.port = stoul(m_port);   
        
        //redis_conn_opt.db = 1;
        // 完成链接
        m_redis_client = make_shared<Redis>(redis_conn_opt);    
        
        m_subscriber = std::make_shared<Subscriber>( m_redis_client->subscriber());

        auto askcmd = g_msgpump_conf.getCmdquerypara();
        for( auto itr = askcmd.begin(); itr != askcmd.end(); ++itr )
        {
            m_subscriber->subscribe( itr->first );
        }

        
        m_subscriber->on_message( [this]( string channel, string msg)
        {
            cout << "on ask cmd arrived...."<< channel << endl;
            action(channel);
        });
        }
    catch(const Error& e )
    {       
        return false;
    }   
     
    return true;
}

bool RedisListener::disconnect()
{    
    return true;
}



void RedisListener::action( string _ask_topic )
{
    // static unsigned long i_times = 0;
    // string cur_tm = to_string((uintmax_t)time(NULL) );
    // string result = m_topic + string(" Now times is:") + to_string(i_times++) + "..........";
    // result+=cur_tm;            
    // return result;
    this->m_msg_pump->receiveAskSignal( _ask_topic);
}

void RedisListener::listening()
{
    while( true )
    {
        try{
            m_subscriber->consume();
           // cout <<"接收到一条指令........"<<endl;
        }catch( const TimeoutError& e)
        {
            continue;
        }catch( const Error &err )
        {
            cout <<"指令监听器错误，重启监听........." ;
            this_thread::sleep_for( chrono::milliseconds(2000)) ;
        }
    }
    
}
    

