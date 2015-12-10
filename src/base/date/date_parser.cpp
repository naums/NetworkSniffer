/****
 * Developed by Stefan Naumann, 2013
 * email: snaumann2912@yahoo.de
**/

#include "date.h"
#include "date_parser.h"

#define RFC822 1
#define RFC850 2
#define ASCTIME 3

namespace base
{
    date* date_parser::parse ( const char* datestring )
    {
        int rfc = date_parser::determine_rfc ( datestring );
        
        int day, month, year, hours, min, sec;
        
        // 6.11.1994 8:49:37 GMT
        if (rfc == RFC822)      // Sun, 06 Nov 1994 08:49:37 GMT
        {
            day = date_parser::atoi (&datestring[5], 2);
            month = date_parser::nameToMonth(&datestring[8], 3);
            year=date_parser::atoi (&datestring[12], 4);
            hours=date_parser::atoi (&datestring[17], 2);
            min=date_parser::atoi (&datestring[20], 2);
            sec=date_parser::atoi (&datestring[23], 2);
        }
        else if (rfc == RFC850) // Sunday, 06-Nov-94 08:49:37 GMT
        {
            day = date_parser::atoi (&datestring[8], 2);
            month = date_parser::nameToMonth(&datestring[11], 3);
            year=date_parser::shortToYear (&datestring[15], 2);
            hours=date_parser::atoi (&datestring[18], 2);
            min=date_parser::atoi (&datestring[21], 2);
            sec=date_parser::atoi (&datestring[24], 2);
        }
        else // rfc == ASCTIME  // Sun Nov  6 08:49:37 1994
        {
            day = date_parser::atoi (&datestring[8], 2);
            month = date_parser::nameToMonth(&datestring[4], 3);
            year=date_parser::atoi (&datestring[20], 4);
            hours=date_parser::atoi (&datestring[11], 2);
            min=date_parser::atoi (&datestring[14], 2);
            sec=date_parser::atoi (&datestring[17], 2);
        }
        
        date* dt = new date();
        dt->mktime(day, month, year, hours, min, sec);
        
        return dt;
    }
    
    int date_parser::determine_rfc ( const char* datestring )
    {
        int komma=0;
        for (int i=0; datestring[i]; i++)
        {
            if ((datestring[i]<'a' || datestring[i]>'z') &&
                (datestring[i]<'A' || datestring[i]>'Z'))
            {
                komma=i;
                break;
            }
        }
        
        if (komma>3)
        {
            return RFC850;
        }
        else
        {
            if (datestring[komma]==',')
            {
                return RFC822;
            }
            return ASCTIME;
        }
    }
    
    int date_parser::atoi ( const char* str, int length ) 
    {
        int num=0;
        for (int i=0; str[i] && i<length; i++)
        {
            if (str[i]==' ') continue;
            if (str[i]<'0' || str[i]>'9')
            {
                return -1;
            }
        
            num*=10;
            num+=str[i]-'0';
        }
        
        return num;
    }
    int date_parser::nameToMonth ( const char* str, int length )
    {
        switch (str[0])
        {
            case 'J': case 'j':
                if (str[1]=='a')
                    return 1;       // Jan
                else if (str[1]=='u' && str[2]=='l')
                    return 7;       // Jul
                else if (str[1]=='u' && str[2]=='n')
                    return 6;       // Jun
            case 'F': case 'f':
                return 2;           // Feb
            case 'M': case 'm':
                if (str[1]=='a')
                {
                    if (str[2]=='r')
                    {
                        return 3;   // Mar
                    }
                    else if (str[2]=='y')
                    {
                        return 5;   // May
                    }
                }
            case 'A': case 'a':
                if (str[1]=='u')
                    return 8;       // Aug
                else if (str[1]=='p')
                    return 4;       // Apr
            case 'S': case 's':
                return 9;           // Sep  
            case 'O': case 'o':
                return 10;          // Oct
            case 'N': case 'n':
                return 11;          // Nov
            case 'D': case 'd':
                return 12;          // Dec
            default: return 1;      // default return: 1 ==> Jan
        }   
    }
    
    int date_parser::shortToYear ( const char* str, int length )
    {
        int num = date_parser::atoi(str, length);
        return (num+1900);
    }
}
