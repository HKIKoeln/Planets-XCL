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
#ifndef __CLTREE
#define __CLTREE
#include "Defines.h"
#include <QString>
#include <QList>
#include <QMap>
#include "XCLException.h"
#include "XCLParsingExpression.h"


template<class Object>
class XCLTree
{
public:
  XCLTree(){}
  ~XCLTree();
  const QMap<QString,Object>& getMap(){return map;};
  void insert(const QString& k, const Object& obj,BOOL multiple=TRUE);
  void insert(XCLTree<Object>* t);
 // void replace(QString k, Object obj);
  void remove(QString k);
  QList<Object>* get(const QString& k);
private:
    QMap<QString,Object> map;
};


template<class Object>
XCLTree<Object>::~XCLTree()
{

}


template<class Object>
void XCLTree<Object>::insert(const QString& k, const Object& obj,BOOL multiple)
{

if(!multiple)
  map.insert(k,obj);
else
  map.insertMulti(k,obj);
}

template<class Object>
void XCLTree<Object>::insert(XCLTree<Object>* t)
{
QMap<QString,Object> om=t->getMap();
QMapIterator<QString, Object> i(om);
 while (i.hasNext()) 
{
     i.next();
     insert(i.key(),i.value(),true);
 }
}

template<class Object>
void XCLTree<Object>::remove(QString key)
{
map.remove(key);
}



template<class Object>
QList<Object>* XCLTree<Object>::get(const QString& k)
{
QList<Object> values = map.values(k);

if(values.isEmpty())
	throw XCLException("Can not find "+k+"\n");
QList<Object>* list = new QList<Object>();

for(int i=values.size()-1;i>=0;--i)
{
list->append(values.at(i));
}

return list;
}


#endif

