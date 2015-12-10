/** \file debug.cpp
 * \author Stefan Naumann
 * \date 2013
 **/

#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include "../date/date.h"
#include "debug.h"

namespace base 
{
    void abort(const char* txt, ...)
    {
        va_list list;
        va_start(list, txt);
        date* dat=new date();
        char* dt=dat->toString();
        #ifdef COMPILER_DEBUG_FILE
        FILE* f=fopen("debug/debug.log", "a");
        #endif
        
        char* buffer=(char*) malloc(1024*sizeof(char));
        vsprintf (buffer, txt, list);
        fprintf (stderr, " -- CRITICAL [%s]: %s\n", dt, buffer);
        #ifdef COMPILER_DEBUG_FILE
        if (f)
        {
            fprintf(f, " -- CRITICAL [%s]: %s\n", dt, buffer);
            fclose(f);
        }
        #endif
        
        free(dt);
        free(buffer);
        va_end(list);
        
        //exit(EXIT_FAILURE);
    }

    void error(const char* txt, ...)
    {
        va_list list;
        va_start(list, txt);
        date* dat=new date();
        char* dt=dat->toString();
        #ifdef COMPILER_DEBUG_FILE
        FILE* f=fopen("debug/debug.log", "a");
        #endif
        
        char* buffer=(char*) malloc(1024*sizeof(char));
        vsprintf (buffer, txt, list);
        fprintf (stderr, " -- ERROR [%s]: %s\n", dt, buffer);
        #ifdef COMPILER_DEBUG_FILE
        if (f)
        {
            fprintf(f, " -- ERROR [%s]: %s\n", dt, buffer);
            fclose(f);
        }
        #endif
        
        free(dt);
        free(buffer);
        va_end(list);
    }

    void warn(const char* txt, ...)
    {
        va_list list;
        va_start(list, txt);
        date* dat=new date();
        char* dt=dat->toString();
        #ifdef COMPILER_DEBUG_FILE
        FILE* f=fopen("debug/debug.log", "a");
        #endif
        
        char* buffer=(char*) malloc(1024*sizeof(char));
        vsprintf (buffer, txt, list);
        fprintf (stderr, " -- WARNING [%s]: %s\n", dt, buffer);
        #ifdef COMPILER_DEBUG_FILE
        if (f)
        {
            fprintf(f, " -- WARNING [%s]: %s\n", dt, buffer);
            fclose(f);
        }
        #endif
        
        free(dt);
        free(buffer);
        va_end(list);
    }

    void log(const char* txt, ...)
    {
        va_list list;
        va_start(list, txt);
        date* dat=new date();
        char* dt=dat->toString();
        #ifdef COMPILER_DEBUG_FILE
        FILE* f=fopen("debug/debug.log", "a");
        #endif
        
        char* buffer=(char*) malloc(1024*sizeof(char));
        vsprintf (buffer, txt, list);
        fprintf (stderr, " -- NOTICE [%s]: %s\n", dt, buffer);
        #ifdef COMPILER_DEBUG_FILE
        if (f)
        {
            fprintf(f, " -- NOTICE [%s]: %s\n", dt, buffer);
            fclose(f);
        }
        #endif
        
        free(dt);
        free(buffer);
        va_end(list);
    }
}
