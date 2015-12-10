/****
 * Developed by Stefan Naumann, 2013
 * email: snaumann2912@yahoo.de
**/

#ifndef BASE_DATE_PARSER
#define BASE_DATE_PARSER

#include "date.h"

namespace base { 

    /**
     * \brief class which holds only static functions for parsing the date-expressions
     * possible to get in HTTP-Request. (see RFC 2616)
     */
    class date_parser
    {
        public:
            /**
             * \brief parses the date string given. Does only support:
             * - RFC 822
             * - RFC 850
             * - ASCTIME -Format
             * \param datestring string to parse
             * \return valid date-object representing the parsed date.
             */
            static date* parse ( const char* datestring );
            /**
             * \brief determines the date-format by looking at the first two words.
             * \param datestring full datestring (see parse)
             * \return a number representing the RFC. 1 => RFC822, 2 => RFC850, 3 => ASCTIME
             */
            static int determine_rfc ( const char* datestring );
            
            /**
             * \brief convertes a short name like 'Nov' to the representing number (eg. 11)
             * \param str string-pointer at the correct place
             * \param length length of part of the string to be read
             * \return number representing the month (default value (on failure) is 1 (Jan))
             */
            static int nameToMonth ( const char* str, int length );
            /**
             * \brief convertes a short year-name (eg. 94) to a 4-digit year (1994)
             * \param str string-pointer at the correct place
             * \param length length of part of the string to be read
             * \return 4-digit year-number
             */
            static int shortToYear ( const char* str, int length );
            
            /**
             * \brief convertes a char-number to real int-numbers. No support for negative numbers. 
             * Optimized for usage with dates.
             * \param str string-pointer at the correct place
             * \param length length of part of the string to be read
             * \return the number given as int
             */
            static int atoi ( const char* str, int length );
    };
    
}

#endif
