# include <cstdlib>
# include <iostream>
# include <iomanip>
# include <ctime>
# include <string>
//using namespace std;

# include "timestamp.h"

//****************************************************************************80

double cpu_time ( void )

//****************************************************************************80
//
//  Purpose:
//
//    CPU_TIME returns the current reading on the CPU clock.
//
//  Modified:
//
//    06 June 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Output, double CPU_TIME, the current reading of the CPU clock, in seconds.
//
{
  double value;

  value = ( double ) clock ( ) / ( double ) CLOCKS_PER_SEC;

  return value;
}
//****************************************************************************80

void timestamp ( void )

//****************************************************************************80
//
//  Purpose:
//
//    TIMESTAMP prints the current YMDHMS date as a time stamp.
//
//  Example:
//
//    31 May 2001 09:45:54 AM
//
//  Modified:
//
//    24 September 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    None
//
{
# define TIME_SIZE 40

  static char time_buffer[TIME_SIZE];
  const struct tm *tm;
  size_t len;
  time_t now;

  now = time ( NULL );
  tm = localtime ( &now );

  len = strftime ( time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm );

  std::cout << time_buffer << "\n";

  return;
# undef TIME_SIZE
}
//****************************************************************************80

std::string timestring ( void )

//****************************************************************************80
//
//  Purpose:
//
//    TIMESTRING returns the current YMDHMS date as a string.
//
//  Example:
//
//    31 May 2001 09:45:54 AM
//

{
# define TIME_SIZE 40

  const struct tm *tm;
  size_t len;
  time_t now;
  char *s;

  now = time ( NULL );
  tm = localtime ( &now );

  s = new char[TIME_SIZE];

  len = strftime ( s, TIME_SIZE, "%d %m %Y %H:%M:%S", tm );
 // for formatting options see:
 // http://www.cplusplus.com/reference/clibrary/ctime/strftime.html   [VH]
  std::string str= std::string(s);
  delete s;
  return str;
# undef TIME_SIZE
}
