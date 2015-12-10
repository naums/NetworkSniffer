#ifndef NAUMS_FILE_DIR
#define NAUMS_FILE_DIR

#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>

namespace base { namespace file {

    /**
     * \brief enum that describes the type of a file (struct dirent), see man readdir()
     **/
    enum filetype
    {
        FT_BLK =  DT_BLK,    ///< File is block device
        FT_CHR =  DT_CHR,    ///< File is character device
        FT_DIR =  DT_DIR,    ///< File is directory
        FT_FIFO = DT_FIFO,   ///< File is a named pipe (FIFO)
        FT_LNK =  DT_LNK,    ///< File is symbolic link
        FT_REG =  DT_REG,    ///< File is regular file
        FT_SOCK = DT_SOCK,   ///< File is Unix domain socket
        FT_UNKNOWN = DT_UNKNOWN    ///< unknown file type
    };

    /**
     * \brief class for handling Directory opening and closing as well as getting (some) files from it 
     **/
    class Dir
    {
        private:
            /// handle of the directory
            DIR* hdir;
            /// handle of the current file ( iterate with next / read )
            struct dirent *hfile;
            
            /**
             * \brief constructs the object
             * \param[in] hdir the handle of the opened directory
             **/
            Dir( DIR* hdir );

            
        public:
            /// closes the directory
            ~Dir();
            
            /**
             * \brief reads one entry of the directory, sets the hfile-handle further and returns the filename
             * \return the filename of the next file
             * \sa next()
             **/
            const char* read();
            /**
             * \brief reads one entry of the directory, sets the hfile-handle further and returns the filename
             * \return the filename of the next file
             * \sa read()
             **/
            const char* next ();
             
            /**
             * \brief opens a directory and returns the Dir-object if sucessfull
             * \param[in] filepath the path to the folder, which shall be opened
             * \return pointer to Dir-object if opening was successfull, NULL otherwise
             **/
            static Dir* open( const char* filepath );
            /**
             * \brief returns the type of the current file
             * \return type of current file
             **/
            enum filetype type ( );
            /**
             * \brief closes the directory
             **/
            void close();
    };

}}

#endif
