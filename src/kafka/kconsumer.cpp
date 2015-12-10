#include "kuser.h"

#include <cstring>
#include <iostream>
#include <cstdio>
#include <librdkafka/rdkafkacpp.h>

namespace network { namespace kafka {

    struct network::trace* getAllByPartition ( network::kafka::kconsumer* cons )
    {
        struct network::trace* 
    }

    kconsumer::kconsumer ( const char* host, const char* topic )
    {
        this->host = host;
        this->topic_str = topic;
        
        this->partition = 0;
        this->start_offset = RdKafka::Topic::OFFSET_BEGINNING;
        this->started = false;
        
        // event callback
        /*ExampleEventCb ex_event_cb;
        conf->set("event_cb", &ex_event_cb, errstr);
        
        // delivery callback
        ExampleDeliveryReportCb ex_dr_cb;
        conf->set("dr_cb", &ex_dr_cb, errstr);*/
    }
    
    bool kconsumer::setStartOffset ( int32_t offset )
    {
        this->start_offset = offset;
    }
    
    kconsumer* kconsumer::create ( const char* host, const char* topic )
    {
        kconsumer* tmp = new kconsumer ( host, topic );
        std::string errstr;
        
        tmp->conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
        tmp->tconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);
        tmp->conf->set("metadata.broker.list", tmp->host, errstr);
        
        tmp->consumer = RdKafka::Consumer::create(tmp->conf, errstr);
        if (!tmp->consumer) 
        {
            delete tmp;
            return NULL;
        }
        std::cout << ">> Created consumer " << tmp->consumer->name() << std::endl;
        
        tmp->topic = RdKafka::Topic::create(tmp->consumer, tmp->topic_str, tmp->tconf, errstr);
        if (!tmp->topic)
        {
            delete tmp;
            return NULL;
        }
        std::cout << ">> Created topic " << tmp->topic->name() << std::endl;
        return tmp;
    }
    
    kconsumer::~kconsumer ()
    {
        this->consumer->stop(topic, partition);

        this->consumer->poll(1000);

        delete this->topic;
        delete this->consumer;
        RdKafka::wait_destroyed(5000);
    }
    
    bool kconsumer::start ()
    {
        RdKafka::ErrorCode resp = consumer->start(this->topic, this->partition, this->start_offset);
        if (resp != RdKafka::ERR_NO_ERROR) 
        {
            std::cerr << "Failed to start consumer: " << RdKafka::err2str(resp) << std::endl;
            return false;
        }
        started = true;
        return true;
    }
    
    bool kconsumer::stop()
    {
        consumer->stop(this->topic, this->partition);
        started = false;
        consumer->poll(1000);
        return true;
    }
    
    bool kconsumer::setHost ( const char* host )
    {
        this->host=host;
        std::string errstr;
        conf->set("metadata.broker.list", this->host, errstr);
        
        RdKafka::Consumer *consumer = RdKafka::Consumer::create(this->conf, errstr);
        if (!consumer)
            return NULL;
            
        this->setTopic ( this->topic_str );
        
        RdKafka::Consumer* prod=this->consumer;
        this->consumer = consumer;
        delete prod;
        
        return true;
    }
    
    const char* kconsumer::getHost ()
    {
        return this->host;
    }
    
    bool kconsumer::setTopic ( const char* topic )
    {
        std::string errstr;
        this->topic_str = topic;
        this->topic = RdKafka::Topic::create(this->consumer, topic, this->tconf, errstr);
        return true;
    }
    
    const char* kconsumer::getTopic ()
    {
        return this->topic_str;
    }
    
    RdKafka::Message* kconsumer::recv ( )
    {
        if (!this->started)
            this->start();
    
        RdKafka::Message *msg = consumer->consume(this->topic, 
                                        this->partition, 
                                        1000);
        switch (msg->err())
        {
            case RdKafka::ERR_NO_ERROR:
                /* Real message */
                std::cerr << "Read msg at offset " << msg->offset() << std::endl;
                if (msg->key()) 
                {
                    std::cerr << "Key: " << *msg->key() << std::endl;
                }
                printf("%.*s\n", static_cast<int>(msg->len()),
                                 static_cast<const char *>(msg->payload()));
                break;

            default:
                /* Errors */
                std::cerr << "Consume failed: " << msg->errstr() << std::endl;
                return NULL;
        }
        
        return msg;
    }
}}
