/** \file debug.h
 * \brief provides some basic-debug-functions (used in OpenCL-Wrapper)
 * \note originally developed for a private HTTP-Server project
 * \author Stefan Naumann
 * \date 2013
 **/

#ifndef _BASE_DEBUG
#define _BASE_DEBUG

namespace base {

    /**
     * \brief outputs error debug-logs.
     * \param txt error message to be written to file and stdout (printf-string)
     * \param ... a printf-like variable structure
     * \warning does not exit the program!
     */
    void error ( const char* txt, ... );
    /**
     * \brief outputs error debug-logs and ends the program.
     * \param txt error message to be written to file and stdout (printf-string)
     * \param ... a printf-like variable structure
     */
    void abort ( const char* txt, ... );
    /**
     * \brief outputs warning debug-logs.
     * \param txt error message to be written to file and stdout (printf-string)
     * \param ... a printf-like variable structure
     */
    void warn  ( const char* txt, ... );
    /**
     * \brief outputs debug-logs.
     * \param txt error message to be written to file and stdout (printf-string)
     * \param ... a printf-like variable structure
     */
    void log   ( const char* txt, ... );
    
}

#endif
