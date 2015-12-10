#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "file.h"

namespace base { namespace file {

    int filesize(const char* filename)
    {
        FILE* f=fopen(filename, "r");
        size_t size=filesize(f);
        fclose(f);
        return size;
    }

    int filesize(FILE* f)
    {
        size_t filesize=0;
        if (f)
        {
            fseek(f, 0, SEEK_END);
            filesize=ftell(f);
            fseek(f, 0, SEEK_SET);
            return filesize;
        }   
        else
            return 0;
    } 
    
    int size ( FILE* f )
    {
        return filesize(f);
    }
    
    unsigned int hash(const char* filepath)
    {
        FILE* f=fopen(filepath, "r");
        if (!f)
            return 0;
        int value=size(f);
        char* buffer=(char*) malloc(value);
        fread(buffer, 1, value, f);
        for (int i=0; buffer[i]; i++)
        {
            value+=buffer[i];
        }
        return value;        
    }
    
    bool isfile(const char* filepath)
    {
        FILE* f=fopen(filepath, "r");
        if (!f)
            return false;
        
        fclose(f);
        return true;
    }
    
    const char* extention ( const char* filepath )
    {
        int size=strlen(filepath);
        
        for (int i=(size-1); i>=0; i--)
        {
            if (filepath[i]=='.')
            {
                return &filepath[(i+1)];
            }
            else if (filepath[i]=='/' || filepath[i]=='\\')
            {
                return "";
            }
        }
        
        return "";
    }
    
    char* readfile(const char* filename, size_t* size)
    {
        FILE* f=fopen(filename, "r");
        if (f==NULL)
            return NULL;
        char* ptr=readfile(f, size);
        fclose(f);
        return ptr;
    }

    char* readfile(FILE* f, size_t* size)
    {
        if (f)
        {
            size_t fsize=filesize(f);
            if (fsize<1)
                return NULL;
            if (size!=NULL)
                *size=fsize;
            
            char* result=(char*) malloc(fsize*sizeof(char) + 1);
            fread(result, 1, fsize, f);
            result[fsize]='\0';
            return result;
        }
        else
        {
            return NULL;
        }
    }
}}
