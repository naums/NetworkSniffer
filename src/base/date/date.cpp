/****
 * Developed by Stefan Naumann, 2013
 * email: snaumann2912@yahoo.de
**/

#include <ctime>
#include <cstdlib>
#include <cstdio>
#include "date.h"

#define BASE_DATE_BUFF_MAXLENGTH 30

namespace base {
    
    date::~date () 
    {
        // nothing
    }

    void date::setTimestamp ( time_t t1 )
    {
        this->timestamp=t1;
    }
    
    void date::addTimestamp ( time_t t1 )
    {
        this->timestamp+=t1;
    }

    time_t date::mktime ( int day, int month, int year, int hour, int min, int sec )
    {
        struct tm timestruct;
        if (hour>23)
            hour=0;
        
        timestruct.tm_sec=sec;
        timestruct.tm_min=min;
        timestruct.tm_hour=hour;
        
        timestruct.tm_year=(year-1900);
        timestruct.tm_mday=day;
        timestruct.tm_mon=--month;
        timestruct.tm_isdst=-1;
        
        this->timestamp=::mktime(&timestruct);
        return this->timestamp;
    }
    
    char* date::toString (enum date_output opt)
    {
        struct tm* timestruct = localtime ( &this->timestamp );
        char* buffer=(char*) malloc ( BASE_DATE_BUFF_MAXLENGTH );
        switch (opt)
        {
            case DDMMYYYY:
                strftime ( buffer, BASE_DATE_BUFF_MAXLENGTH, "%d.%m.%Y", timestruct );
                break;
            case MMDDYYYY:
                strftime ( buffer, BASE_DATE_BUFF_MAXLENGTH, "%m-%d-%Y", timestruct );
                break;
            case YYYYMMDD:
                strftime ( buffer, BASE_DATE_BUFF_MAXLENGTH, "%Y-%m-%d", timestruct );
                break;
            case DDMMYYHHMMSS: case DEFAULT:
                strftime ( buffer, BASE_DATE_BUFF_MAXLENGTH, "%d.%m.%Y %H:%M:%S", timestruct );
                break;
            case HHMMSS:
                strftime ( buffer, BASE_DATE_BUFF_MAXLENGTH, "%H:%M:%S", timestruct );
                break;
            case WOCDDMONYYYYHHMMSSZONE: case WEB:
                strftime ( buffer, BASE_DATE_BUFF_MAXLENGTH, "%a, %d %b %Y %H:%M:%S %Z", timestruct );
                break;
            default:
                free(buffer);
                return NULL;
        }
        
        return buffer;
    }
    
    time_t date::getTimestamp ()
    {
        return this->timestamp;
    }
    
    time_t date::time()
    {
        return ::time(NULL);
    }
    
}
