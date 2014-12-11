/*

    * This module is part of the XCL software system
    *
    * Written by Sebastian Beyl, Volker Heydegger, Jan Schnasse, Manfred Thaller, Elona Weiper 2006 - 2009
    *
    * Copyright (C) 2006 - 2009 Sebastian Beyl, Volker Heydegger, Jan Schnasse, Manfred Thaller, Elona Weiper 
    *
    * This program is free software; you can redistribute it and/or modify
    * it under the terms of the GNU General Public License as published by
    * the Free Software Foundation; either version 2 of the License, or
    * (at your option) any later version.
    *
    * This program is distributed in the hope that it will be useful,
    * but WITHOUT ANY WARRANTY; without even the implied warranty of
    * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    * GNU General Public License for more details.
    *
*/
#ifndef __MYXMLSYNTAXHIGHLIGHTER_H
#define __MYXMLSYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
/** MyXMLSyntaxHighlighter färbt Tags ein.
 *
 */
class MyXMLSyntaxHighlighter : public QSyntaxHighlighter
{
private:
	int  w;
	int s;
 public:
    MyXMLSyntaxHighlighter(QTextEdit* edit, int size, int weight):QSyntaxHighlighter(edit)
    {
	w=weight;
	s=size;
    }
    int highlightParagraph ( const QString & text, int endStateOfLastPara )
    {
	int start=0;
	int end=0;
	while((start=text.indexOf("<",end))!=-1)
	{
	    if((end=text.indexOf(">",start+1))!=-1)
            {
	    setFormat(start,end-start+1,QFont("Times"));
            setFormat(start,end-start+1,QColor(0,0,255));
		}
	    else
		end=start+1;
	  
	}
	return 0;
        // USELESS Block! Parameter is not in use, for less compiler-warnings only!
        // int endStateOfLastPara
        {
          endStateOfLastPara = 0;
        }
    }
void highlightBlock(const QString &text){
int start=0;
	int end=0;
	while((start=text.indexOf("<",end))!=-1)
	{
	    if((end=text.indexOf(">",start+1))!=-1)
            {
	    setFormat(start,end-start+1,QFont("Times",s,w));
            setFormat(start,end-start+1,QColor(255,0,0));
		}
	    else
		end=start+1;
	  
	}

}
};
#endif
