#include "config.h"

#include <cstdio>
#include <cstdlib>

config::config ( const char* srv, const char* usr, const char* passwd, 
                 const char* dbname, const char* trace, const char* init, 
                 const char* app )
{
    mysql_server = srv;
    mysql_username = usr;
    mysql_passwd = passwd;
    mysql_dbname = dbname;
    mysql_trace_table = trace;
    mysql_init_table = init;
    mysql_app_table = app;
    
    running=true;
}

config::~config ()
{
    //if ( kafka_server != NULL )
    //    free (kafka_server);
    //if ( kafka_config_topic != NULL )
    //    free (kafka_config_topic);
    //if ( kafka_trace_topic != NULL )
    //    free (kafka_trace_topic);
    //if ( kafka_resource_topic != NULL )
    //    free (kafka_resource_topic);
}

void config::setRunning ()
{
    running=true;
}

void config::exit()
{
    running=false;
}

/*

cfgThread::cfgThread ( config* c )
{
    this->cfg = c;
    this->cons = network::kafka::kconsumer::create ( cfg->kafka_server, cfg->kafka_config_topic );
}

cfgThread::~cfgThread()
{
    this->stop();
    if (this->cons)
        delete this->cons;
}

void cfgThread::setCfg ( config* c )
{
    this->cfg = c;
    this->cons->setTopic ( cfg->kafka_config_topic );
    this->cons->setHost ( cfg->kafka_server );
}

void* cfgThread::run ()
{
    while (cfg->running)
    {
        RdKafka::Message* mesg = this->cons->recv ();
        if (mesg)
            printf (" -- [CFG] %s\n", (char*) mesg->payload());
    }
}*/


