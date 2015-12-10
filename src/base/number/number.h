#ifndef NAUMS_BASE_NUMBERSCONVERSION
#define NAUMS_BASE_NUMBERSCONVERSION

namespace base {

    /**
     * \brief converts an unsigned char to a hexadecimal presentation
     * \param[in] byte the byte to be converted
     * \return malloc'd string containing the hexadecimal presentation
     * \note please free the memory after usage!
     **/
    char* btohex ( const unsigned char byte );
    
    /** 
     * \brief converts an unsigned char to a decimal presentation
     * \param[in] byte the byte to be converted
     * \return malloc'd string containing the decimal presentation
     * \note please free the memory after usage!
     **/
    char* btodec ( const unsigned char byte );   
}

#endif
