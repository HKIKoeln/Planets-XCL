# include <cstdlib>
# include <iostream>
# include <iomanip>
# include <ctime>
# include <string>

double cpu_time(void)
{
  double value;

  value = ( double ) clock ( ) / ( double ) CLOCKS_PER_SEC;

  return value;
}

void timestamp(void)
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


std::string timestring(void)
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
