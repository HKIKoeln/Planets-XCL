#ifndef __PRINTER_H__
#define __PRINTER_H__

#include <QString>
#include <QTextStream>
#include <QFile>
class Printer
{

public:
	static void print(QString str)
	{
		;
		QTextStream textstr(stdout);
 	        textstr<<str;

	}	

};

#endif
