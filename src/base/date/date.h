/****
 * Developed by Stefan Naumann, 2013
 * email: snaumann2912@yahoo.de
**/

#ifndef BASE_DATE_DATE
#define BASE_DATE_DATE

#include <ctime>

namespace base {

    /// \enum determines the format of the date outputted by date::toString();
    enum date_output
    {
        DDMMYYYY, ///< Day.Month.Year
        MMDDYYYY, ///< Month-Day-Year
        YYYYMMDD, ///< Year-Month-Day
        DDMMYYHHMMSS, ///< Day.Month.Year Hour:Minutes:Seconds
        HHMMSS, ///< Hours:Minutes:Seconds
        DEFAULT, ///< Day.Month.Year Hour:Minutes:Seconds (default state) 
        WOCDDMONYYYYHHMMSSZONE, ///< Weekday, Day Monthname Year Hours:Minutes:Seconds Timezone (Webview)
        WEB ///< Weekday, Day Monthname Year Hours:Minutes:Seconds Timezone (Webview)
    };

    /**
     * \brief date class for describing dates.
     */
    class date 
    {
        private:
            /// timestamp to be hold by date-class
            time_t timestamp;
        
        public:
            /**
             * \brief constructor. Sets the timestamp of date-class
             * \param t1 timestamp to be set (default is the current time!)
             */
            date ( time_t t1=::time(NULL) ) : timestamp (t1) {};
            /**
             * \brief destructor. Deletes date-object. Empty
             */
            ~date ();
            
            /**
             * \brief sets timestamp
             * \param t1 the timestamp to be set
             * \return none
             */
            void setTimestamp ( time_t t1 );
            /**
             * \brief adds (as in +) the given timestamp to the one saved in date-object
             * \param t1 the timestamp to be added
             * \return none
             */
            void addTimestamp ( time_t t1 );
            
            /**
             * \brief creates the timestamp, saves it, and returns it (by getting date-describing
             * numbers)
             * \param day day of the month (1-31)
             * \param month month of the year (1-12)
             * \param year year since year 0
             * \param hour hour since 0:00 (0-23)
             * \param min minutes since the last full hour (0-59)
             * \param sec seconds since the last full minute (0-59)
             * \return calculated timestamp
             */
            time_t mktime ( int day, int month, int year, int hour=0, int min=0, int sec=0 );
            /**
             * \brief transforms the timestamp into a string
             * \param opt see enum date_output for more information about the options
             * \return the cstring containing the human readable date
             */
            char* toString (enum date_output opt=DEFAULT);
            
            /**
             * \brief returns the currently saved timestamp
             * \params <none>
             * \return the currently saved timestamp
             */
            time_t getTimestamp ();
            /**
             * \brief static function! Returns the current time (wrapper for ::time(NULL))
             * \params <none>
             * \return current timestamp 
             */
            static time_t time();
    };
}

#endif
