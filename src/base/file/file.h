                                                                            
#ifndef NAUMS_BASE_FILES
#define NAUMS_BASE_FILES

#include <cstdio>

/// namespace with file-handling-functions
namespace base { namespace file {

    /**
     * \brief returns the size in Bytes of a given file
     * \param[in] filename the path to the file
     * \return the number of bytes the file contains
     * \author Stefan Naumann
     * \date 04.06.2014
     **/
    int filesize(const char* filename);
    /**
     * \brief returns the size in Bytes of a given file
     * \param[in] f filepointer to the file
     * \return the number of bytes the file contains
     * \author Stefan Naumann
     * \date 04.06.2014
     **/
    int filesize(FILE* f);  
    /**
     * \brief returns the size in Bytes of a given file
     * \param[in] f filepointer to the file
     * \return the number of bytes the file contains
     * \author Stefan Naumann
     * \date 04.06.2014
     **/
    int size(FILE* f);
    
    
    /// creates a hash-Value for that specific file
    /// isn't secure, isn't pretty, but works. Hopefully.
    unsigned int hash(const char* filepath);
    
    bool isfile(const char* filepath);
    
    const char* extention ( const char* filepath );
    
    /**
     * \brief reads a file completely and returns a pointer to the string read. 
     * \param[in] filename the path to the file
     * \param[out] size the size of the file / output string
     * \return the malloc'd string
     * \warn the string returned is malloc'd please free the space after you're done!
     * \author Stefan Naumann
     * \date 04.06.2014
     **/
    char* readfile(const char* filename, size_t* size=NULL);
    /**
     * \brief reads a file completely and returns a pointer to the string read. 
     * \param[in] f filepointer to the file
     * \param[out] size the size of the file / output string
     * \return the malloc'd string
     * \warn the string returned is malloc'd please free the space after you're done!
     * \author Stefan Naumann
     * \date 04.06.2014
     **/
    char* readfile(FILE* f, size_t* size=NULL);
}}

#endif
