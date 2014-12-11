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
#ifndef __XCELREADERDATAOBJECT_H
#define __XCELREADERDATAOBJECT_H
#include <QHash>
class XCLSyntaxItem;
class XCLParsingItem;
class QByteArray;


class XCELReaderDataObject
{
public:
XCELReaderDataObject()
{
actGrammar=NULL;
preprocessingGrammar=NULL;
processingGrammar=NULL;
postprocessingGrammar=NULL;
byteArray=NULL;
parseTree=NULL;
grammarIndex=NULL;
}

~XCELReaderDataObject()
{
if(actGrammar!=NULL)delete actGrammar;
if(preprocessingGrammar!=NULL)delete preprocessingGrammar;
if(processingGrammar!=NULL)delete processingGrammar;
if(postprocessingGrammar!=NULL)delete postprocessingGrammar;
if(byteArray!=NULL)delete byteArray;
if(parseTree!=NULL)delete parseTree;
if(grammarIndex!=NULL)delete grammarIndex;
}

XCLSyntaxItem* actGrammar;
XCLSyntaxItem* preprocessingGrammar;
XCLSyntaxItem* processingGrammar;
XCLSyntaxItem* postprocessingGrammar;
QByteArray* byteArray;
XCLParsingItem* parseTree;
QHash<QString,XCLSyntaxExpression*>* grammarIndex;
};
#endif

