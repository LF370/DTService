#include"msgdriverconf.h"
#include<filesystem>
#include<unistd.h>



   
bool MsgPumpConfig::readConfig()
{
    char buffer[1024]={0};
    getcwd(buffer,1024);
    string cwd = buffer;
    cout<< cwd << endl;
    
    
    TiXmlDocument doc;
    string xml_file = "msgdriverconfig.xml";
    bool ret = doc.LoadFile(xml_file);
    if( !ret )
    {           
        return false;
    }
    
    doc.ToDeclaration();

    TiXmlElement *root = doc.RootElement();
    if( !root )
    {
        return false;
    }
    
    TiXmlNode* broker1 = root->FirstChild(SUBSCRIBE_BROKE_NODE); 
    while( broker1 )
    {  
        for( TiXmlNode *child = broker1->FirstChild();child; child = child->NextSibling() )
        {
            m_sbroker_set.insert( make_pair(child->Value(), child->ToElement()->GetText() ));                
        }
        
        broker1 = broker1->ToElement()->NextSibling(SUBSCRIBE_BROKE_NODE);
    }

    
    TiXmlNode* broker = root->FirstChild(PUBLISH_BROKER_NODE);                
        
    while( broker )
    {  
        for( TiXmlNode *child = broker->FirstChild();child; child = child->NextSibling() )
        {
            m_pbroker_set.insert( make_pair(child->Value(), child->ToElement()->GetText() ));                
        }
        
        broker = broker->ToElement()->NextSibling(PUBLISH_BROKER_NODE);
    }



    // 获取轮询指令参数
    TiXmlNode* channel = root->FirstChild(LOOPQUERY_NODE);
    
    while( channel )
    {
        string topic_str;
        double rep_tm;
        string pre_topic_str ;
        for( TiXmlNode *child = channel->FirstChild();child; child = child->NextSibling() )
        {
            
            string tx_v = child->Value();                
            if( tx_v =="topic")
            {
                topic_str = string(LOOPQUERY_NODE) + "-" + child->ToElement()->GetText();
                m_loop_query_map.insert( make_pair(topic_str,0));
                pre_topic_str = topic_str;

            }
            if( tx_v =="reptm")
            {
                rep_tm = stod( child->ToElement()->GetText() );
                m_loop_query_map[pre_topic_str] = rep_tm;
            }              
            
        }        

        channel = channel->ToElement()->NextSibling(LOOPQUERY_NODE);
    }
    

    // 获取命令式查询
    channel = root->FirstChild(CMDQUERY_NODE);
    
    while( channel )
    {
        string topic_str;
        double rep_tm;
        string pre_topic_str ;
        for( TiXmlNode *child = channel->FirstChild();child; child = child->NextSibling() )
        {
            
            string tx_v = child->Value();                
            if( tx_v =="topic")
            {
                  topic_str = string(CMDQUERY_NODE) + "-" + child->ToElement()->GetText();
                m_cmd_query_map.insert( make_pair(topic_str,0));
                pre_topic_str = topic_str;

            }
            if( tx_v =="reptm")
            {
                rep_tm = stod( child->ToElement()->GetText() );
                m_cmd_query_map[pre_topic_str] = rep_tm;
            }              
            
        }        

        channel = channel->ToElement()->NextSibling(CMDQUERY_NODE);
    }
    

    return true;


}


