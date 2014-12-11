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
#include <iostream>
#include "XCLFilter.h"
#include "XCLException.h"



XCLFilter::XCLFilter()
{
}

XCLFilter::XCLFilter(const XCLFilter& f):
    filtername(f.filtername),
    filtervalue(f.filtervalue),
    parameters(f.parameters)
{

}

XCLFilter& XCLFilter::operator=(const XCLFilter& f)
{
if(this!=&f)
  {
    filtername=f.filtername;
    filtervalue=f.filtervalue;
    parameters=f.parameters;
  }
  return *this;
}
XCLFilter::~XCLFilter()
{}


void XCLFilter::setFilterName(const QString& name)
{
  filtername = name;
}


void XCLFilter::setFilterValue(const QString& value)
{
  filtervalue = value;
}

BOOL XCLFilter::isEqual(const XCLFilter& f)
{
if (filtername==f.filtername &&
filtervalue==f.filtervalue) 
       return TRUE;


return FALSE;
}
void XCLFilter::addParam(const QString& paramname, const QString& paramvalue)
{

  QString pname = paramname;
  QString pvalue = paramvalue;


  parameters.insert(pname, pvalue);


}
/*
{
    QString pname = paramname;
    QString pvalue = paramvalue;
    parameters.insert(paramname, paramvalue);  
      
 
}*/

const QString& XCLFilter::getFilterName()
{
  return filtername;
}
const QString& XCLFilter::getFilterValue()
{
  return filtervalue;
}

const QHash<QString,QString> XCLFilter::getParams()
{
  return parameters;
}

void XCLFilter::print(QString format)
{
  std::cout<<format.toStdString()<<"Filtername: "<<filtername.toStdString()<<std::endl;
  std::cout<<format.toStdString()<<"Filtervalue: "<<filtervalue.toStdString()<<std::endl;

  if (!parameters.isEmpty())
  {
    QHashIterator<QString, QString> i(parameters);
    while (i.hasNext())
    {
      i.next();
      std::cout <<format.toStdString()<<"key:"<< i.key().toStdString() << "; value: " << i.value().toStdString() << std::endl;
    }
  }
}

QString XCLFilter::getStringPar(
  const QString              &name,
  bool                             enforce)
{
  QString result;
  if ((result=getParams().value(name)).isEmpty()==true && enforce==true)
    throw XCLException(QObject::tr("No parameter '%1' supplied in postprocessing.").arg(name));


  return result;
}

int XCLFilter::getIntPar(
  const QString           &name,
  bool                          enforce)
{
  QString intermediate;
  int result;
  bool ok;

  try
  {
    intermediate=getStringPar(name,enforce);
  }
  catch(XCLException exception)
  {
	exception.message();
  }

  result=intermediate.toInt(&ok);
  if (ok!=true)
  {
    print();
    throw XCLException(QObject::tr("Parameter '%1' contains illegal value '%2'")
                       .arg(name).arg(intermediate));
   }

  return result;
}
