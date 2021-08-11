#include<vector>
#include<iostream>
#include<algorithm>
#include<iterator>
#include"msgdriverconf.h"
#include"msgpump.h"

using namespace std;
MsgPumpConfig g_msgpump_conf;
int main( void )
{
    
    bool ret = g_msgpump_conf.readConfig();
    if( !ret )
    {
        cout <<"无法读取服务配置文件........."<<endl;
        exit(-1);
    }

    auto broker_para = g_msgpump_conf.getBrokerConf();
    auto loopquery_para = g_msgpump_conf.getLoopquerypara();
    auto cmdquery_para = g_msgpump_conf.getCmdquerypara();

    for( auto itr = broker_para.begin(); itr != broker_para.end(); ++itr )
    {
        cout << itr->first<<":"<< itr->second <<endl;
    }
    for( auto itr = loopquery_para.begin(); itr != loopquery_para.end(); ++itr )
    {
        cout << itr->first<<":"<< itr->second <<endl;
    }
    for( auto itr = cmdquery_para.begin(); itr != cmdquery_para.end(); ++itr )
    {
        cout << itr->first<<":"<< itr->second <<endl;
    }


    MsgPump msg_pump;

    int eng_sz = msg_pump.assemblyMsgEngine();
    if( eng_sz > 0 )
    {
        cout <<"已经完成 "<<eng_sz<<"台消息引擎连线.........."<<endl;

    }

    msg_pump.start();

    


    return 1;
}