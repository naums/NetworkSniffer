#include "pcap/pcap.h"
#include "kafka/kuser.h"
#include "config.h"
#include "sniffer.h"
#include "base/debug/debug.h"

#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <unistd.h>

#define MYSQL_SERVER "10.10.10.141\0"
#define MYSQL_USERNAME "test"
#define MYSQL_PASSWD "passwd"
#define MYSQL_DBNAME "test"

#define MYSQL_TRACE "trace"
#define MYSQL_CONFIG "config"

#define MYSQL_INIT "dev"
/// deprecated
#define MYSQL_APP "application"
#define MYSQL_RSRC "rsrc"

int main ( int argc, char* argv[] )
{
    printf ("Usage: %s [(-l|srv)] [usr] [pass] [dbname] [config] [trace] [rsrc] [init]\n  -l -> List of Devices\n\n", argv[0]);

    /**
     * Config / Command line parameters 
    **/ 
    bool list=false;
    if (argc==2)
    {
        if (strcmp ( argv[1], "-l") == 0)
        {
            network::pcap::list_devices ( ); 
            list=true;
            return EXIT_SUCCESS;
        }
    }
    
    const char* srv    = ((argc >=2 && !list) ? argv[1] : MYSQL_SERVER);
    const char* usr    = (argc >=3 ? argv[2] : MYSQL_USERNAME);
    const char* passwd = (argc >=4 ? argv[3] : MYSQL_PASSWD);
    const char* dbname = (argc >=5 ? argv[4] : MYSQL_DBNAME);
    const char* trace  = (argc >=6 ? argv[5] : MYSQL_TRACE);
    const char* app    = (argc >=7 ? argv[6] : MYSQL_APP);
    const char* init   = (argc >=8 ? argv[7] : MYSQL_INIT);
 
    config* cfg = new config(srv, usr, passwd, dbname, trace, init, app);
    /**
     * starting threads (config, network-sniff)
     **/
    // config
    //cfgThread* tcfg = new cfgThread( cfg );
    //tcfg->start();

    dbstore* db = dbstore::connect ( cfg );

    if (!db)
        base::error("Connection to MySQL could not be established: %s", db->getMySQL()->error());
    #if 1
    char* buffer = network::pcap::start_info();
    db->storeInitial(buffer);
    free(buffer);
    #endif

    #if 0
    sniffer* sn = new sniffer ( cfg, "eth0" );
    sn->run();
    
    while (1) { sleep(1); }
    #endif
    //delete tcfg;
}
