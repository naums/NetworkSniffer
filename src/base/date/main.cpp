/****
 * Developed by Stefan Naumann, 2013
 * email: snaumann2912@yahoo.de
**/

#include <cstdio>
#include <cstdlib>
#include "date.h"
#include "date_parser.h"
#include "dev/test/test.h"

#include <ctime>

using namespace dev::test;
using namespace base;

int main ( int argc, char* argv[] )
{
    date* dt = date_parser::parse ( "Sun, 06 Nov 1994 08:49:37 GMT");
    date* dt2 = new date ( 0 );
    
    testString ("01.01.1970", dt2->toString ( DDMMYYYY ));
    testString ("06.11.1994", dt->toString ( DDMMYYYY));
    testString ("06.11.1994 08:49:37", dt->toString (DDMMYYHHMMSS));
    dt->addTimestamp(60*60*24);
    testString ("07.11.1994 08:49:37", dt->toString (DDMMYYHHMMSS));
    
    testString ("Mon, 07 Nov 1994 08:49:37 CET", dt->toString(WEB));
    
    dt2->setTimestamp(60*60*24*365*10+2*60*60*24);
    
    testString ("01.01.1980", dt2->toString(DDMMYYYY));
    testString ("01-01-1980", dt2->toString(MMDDYYYY));
    testString ("11-07-1994", dt->toString(MMDDYYYY));
    
    date* dt3 = date_parser::parse ( "Sunday, 06-Nov-94 08:49:37 GMT");
    
    testString ("06.11.1994", dt3->toString (DDMMYYYY));
    testString ("Sun, 06 Nov 1994 08:49:37 CET", dt3->toString(WEB));
    testString ("06.11.1994 08:49:37", dt3->toString(DEFAULT));
    
    date* dt4 = date_parser::parse ("Sun Nov  6 08:49:37 1994");
    
    testString ("06.11.1994", dt4->toString (DDMMYYYY));
    testString ("Sun, 06 Nov 1994 08:49:37 CET", dt4->toString(WEB));
    testString ("06.11.1994 08:49:37", dt4->toString(DEFAULT));
    
    delete dt;
    delete dt2;
    delete dt3;
}
