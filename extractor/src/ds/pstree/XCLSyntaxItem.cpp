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
#include "XCLSyntaxItem.h"
#include "XCLSyntaxProperty.h"
#include "XCLSyntaxProcess.h"
#include "XCLSyntaxSymbol.h"


// Constructors/Destructors
//

XCLSyntaxItem::XCLSyntaxItem()
    : XCLSyntaxExpression()
{
  initAttributes();
}


XCLSyntaxItem::XCLSyntaxItem (const XCLSyntaxItem& itm)
    :XCLSyntaxExpression(itm)
{
  initAttributes();
  setItemType(itm.item_type);
  order_type=itm.order_type;
  objectType=itm.objectType;
  setChildren(itm.children);
}


XCLSyntaxItem::~XCLSyntaxItem ( )
{
  _LENGTH size=children.size();
  for (_LENGTH i = 0; i < size ; i++)
  {
    delete children[i];
  }
}


XCLSyntaxItem& XCLSyntaxItem::operator=(const XCLSyntaxItem& itm)
{
  if(this!=&itm)
  {
    XCLSyntaxExpression::operator=(itm);
    setItemType(itm.item_type);
    order_type=itm.order_type;
    objectType=itm.objectType;
    setChildren(itm.children);
  }
  return *this;
}


void XCLSyntaxItem::initAttributes ( )
{
  order_type = SEQUENCE;
  item_type = STRUCTURING;
  objectType="other";
}

void XCLSyntaxItem::setObjectType(const QString& objT)
{
objectType=objT;
}
const QString& XCLSyntaxItem::getObjectType()
{
return objectType;
}
XCLSyntaxElement* XCLSyntaxItem::getLastChild() const
{
  XCLSyntaxElement* expr=children.last();
  short exprType=expr->getElementType();
  if(exprType == XCLSyntaxElement::ITEM_TYPE)
  {
    return new XCLSyntaxItem(*((XCLSyntaxItem*)expr));
  }
  else if(exprType == XCLSyntaxElement::SYMBOL_TYPE)
  {
    return new XCLSyntaxSymbol(*((XCLSyntaxSymbol*)expr));
  }
  else if(exprType == XCLSyntaxElement::PROPERTY_TYPE)
  {
    return new XCLSyntaxProperty(*((XCLSyntaxProperty*)expr));
  }
  else if(exprType == XCLSyntaxElement::PROCESS_TYPE)
  {
    return new XCLSyntaxProcess(*((XCLSyntaxProcess*)expr));
  }
  return NULL;
}


void XCLSyntaxItem::add(const XCLSyntaxItem& expr)
{
  XCLSyntaxItem* item=new XCLSyntaxItem(expr);
  if( (item_type==STRUCTURING))
    item->setParent(this);
  else
    item->setParent(getParent());
  children.append(item);
  item->setIsBigEndian(getIsBigEndian());

}


void XCLSyntaxItem::add(const XCLSyntaxSymbol& expr)
{
  XCLSyntaxSymbol* symbol =new XCLSyntaxSymbol(expr);

  symbol->setIsBigEndian(getIsBigEndian());
  children.append(symbol);
  if ( item_type==STRUCTURING)
    children.last()->setParent(this);
  else
    children.last()->setParent(getParent());

}


void XCLSyntaxItem::add(const XCLSyntaxProperty& expr)
{
  XCLSyntaxProperty* property =new XCLSyntaxProperty(expr);
  property->setIsBigEndian(getIsBigEndian());
  children.append(property);
  if( item_type==STRUCTURING)
    children.last()->setParent(this);
  else
    children.last()->setParent(getParent());
 
}

void XCLSyntaxItem::add(const XCLSyntaxProcess& expr)
{
  children.append(new XCLSyntaxProcess(expr));
  children.last()->setParent(this);
}


enum XCLSyntaxElement::ELEMENT_TYPE XCLSyntaxItem::getElementType()const
{
  return XCLSyntaxElement::ITEM_TYPE;
}


void XCLSyntaxItem::setOrderType(enum ORDER_TYPE ot)
{
  order_type=ot;
}


enum XCLSyntaxItem::ORDER_TYPE XCLSyntaxItem::getOrderType()
{
  return order_type;
}


/**
 * Set the value of children
 * @param new_var the new value of children
 */
void XCLSyntaxItem::setChildren ( const QVector<XCLSyntaxElement*>& es )
{
  children.clear();
  QVector<XCLSyntaxElement*> childs =es;
  QVector<XCLSyntaxElement*>::iterator pos;
  for(pos = childs.begin();pos<childs.end();++pos)
  {
    short exprType=(*pos)->getElementType() ;
    if(exprType== XCLSyntaxExpression::ITEM_TYPE)
    {
      add(*((XCLSyntaxItem*)(*pos)));
    }
    else if(exprType == XCLSyntaxExpression::PROPERTY_TYPE)
    {
      add(*((XCLSyntaxProperty*)(*pos)));
    }
    else if(exprType == XCLSyntaxExpression::SYMBOL_TYPE)
    {
      add(*((XCLSyntaxSymbol*)(*pos)));
    }
    else if(exprType == XCLSyntaxExpression::PROCESS_TYPE)
    {
      add(*((XCLSyntaxProcess*)(*pos)));
    }
  }
}


/**
 * Get the value of children
 * @return the value of children
 */
const QVector<XCLSyntaxElement*>& XCLSyntaxItem::getChildren ( )
{
  return children;
}


/**
 * 
 * @return a vector of all optional children
 */
const QVector<XCLSyntaxElement*> XCLSyntaxItem::getOptionalChildren ( )
{
  QVector<XCLSyntaxElement*> optionalChildren;
  for (int i=0; i<children.size();i++)
  {
    if ( ((XCLSyntaxExpression*)children[i])->getIsOptional() )
      optionalChildren.append( children[i] );
  }
  return optionalChildren;
}


/**
 * @return a vector of all non optional children
 */
const QVector<XCLSyntaxElement*> XCLSyntaxItem::getNonOptionalChildren ( )
{
  QVector<XCLSyntaxElement*> nonOptionalChildren;
  for (int i=0; i<children.size();i++)
  {
    if ( !((XCLSyntaxExpression*)children[i])->getIsOptional() )
      nonOptionalChildren.append( children[i] );
  }
  return nonOptionalChildren;
}


void XCLSyntaxItem::setItemType(const QString& t)
{
  if(t == "structuringItem")
    item_type=STRUCTURING;

  else if(!name.isEmpty())
    item_type=DEFINITION;
  else 
    item_type=CONTEXT;
}


void XCLSyntaxItem::setItemType(enum ITEM_TYPE t)
{
  item_type=t;
}


short XCLSyntaxItem::getItemType()
{
  return item_type;
}


void XCLSyntaxItem::setLength(_LENGTH le)
{

  XCLSyntaxExpression::setLength(le);
}


void XCLSyntaxItem::setStart(_LENGTH s)
{
  XCLSyntaxExpression::setStart(s);
}


void XCLSyntaxItem::setEnd(_LENGTH e)
{
  if(end != LENGTH_MAX)
  {
    XCLSyntaxExpression::setEnd(e);

  }
}


void XCLSyntaxItem::print(QString f)
{
  XCLSyntaxExpression::print(f);
  switch(item_type)
  {
  case STRUCTURING:
    std::cout<<f.toStdString()<<"type: "<<"STRUCTURING"<<std::endl;
    break;
  case CONTEXT:
    std::cout<<f.toStdString()<<"type: "<<"CONTEXT"<<std::endl;
    break;
  case DEFINITION:
    std::cout<<f.toStdString()<<"type: "<<"DEFINITION"<<std::endl;
    break;
  default:
    std::cout<<f.toStdString()<<"type: "<<"not identified"<<std::endl;
  }
  std::cout<<f.toStdString()<<"Order: ";
  switch(order_type)
  {
  case ALL:
    std::cout<<f.toStdString()<<"type: "<<"ALL"<<std::endl;
    break;
  case SEQUENCE:
    std::cout<<f.toStdString()<<"type: "<<"SEQUENCE"<<std::endl;
    break;
  case CHOICE:
    std::cout<<f.toStdString()<<"type: "<<"CHOICE"<<std::endl;
    break;
  default:
    std::cout<<f.toStdString()<<"type: "<<"not identified"<<std::endl;
  }

  QVector<XCLSyntaxElement*>::iterator pos;
  for(pos=children.begin();pos < children.end();++pos)
  {
    XCLSyntaxElement* expr= (*pos);
    if(expr->getElementType() == XCLSyntaxElement::ITEM_TYPE)
    {
      std::cout<<"Item"<<std::endl;
      ((XCLSyntaxItem*)expr)->print(f+"\t");
    }
    else if(expr->getElementType() == XCLSyntaxElement::SYMBOL_TYPE)
    {
      std::cout<<"Symbol"<<std::endl;
      ((XCLSyntaxSymbol*)expr)->print(f+"\t");
    }
    else if(expr->getElementType() == XCLSyntaxElement::PROPERTY_TYPE)
    {
      std::cout<<"Property"<<std::endl;
      ((XCLSyntaxProperty*)expr)->print(f+"\t");
    }
    else if(expr->getElementType() == XCLSyntaxElement::PROCESS_TYPE)
    {
      std::cout<<"Processing"<<std::endl;
      ((XCLSyntaxProcess*)expr)->print(f+"\t");
    }
    else
    {
      std::cout<<"Element"<<std::endl;
    }
  }

  std::cout<<"Ende ITEM "<<getName().toStdString()<<std::endl;
}


XCLSyntaxElement* XCLSyntaxItem::getFirstChild()
{
  return children.first();
}


BOOL XCLSyntaxItem::replace(XCLSyntaxExpression* expr)
{

  for(int i=0;i<children.size();i++)
  {
    if(children.at(i)->getElementType()!=PROCESS_TYPE)
    {
      XCLSyntaxExpression* e=(XCLSyntaxExpression*)children.at(i);

      if(e->getIdentifier()==expr->getIdentifier())
      {
        children.replace(i,expr);

        return TRUE;
      }
      else if(e->getElementType()==XCLSyntaxElement::ITEM_TYPE)
      {
        if(((XCLSyntaxItem*)e)->replace(expr))
          return TRUE;
      }
    }

  }
  return FALSE;
}


BOOL XCLSyntaxItem::remove(XCLSyntaxExpression* expr)
{
  for(int i=0;i<children.size();i++)
  {
    if(children.at(i)->getElementType()==XCLSyntaxElement::ITEM_TYPE)
    {
      XCLSyntaxExpression* e=(XCLSyntaxExpression*)children.at(i);
      if(e->getIdentifier()==expr->getIdentifier())
      {
        children.remove(i);
        return TRUE;
      }
    }

  }
  return FALSE;
}


const QString& XCLSyntaxItem::getInterpretation()
{
  _LENGTH size=children.size();
  for(_LENGTH i=0; i<size;i++)
  {
    if(children.at(i)->getElementType() != XCLSyntaxElement::PROCESS_TYPE)
      return ((XCLSyntaxExpression*)(children.at(i)))->getInterpretation();
  }

  throw XCLException("Undefined interpretation in XCLSyntaxItem::getInterpretation()\n");
}


void XCLSyntaxItem::setByteOrder(const QString& bo)
{
  XCLSyntaxExpression::setByteOrder(bo);
  _LENGTH size=children.size();
  for(_LENGTH i=0; i<size;i++)
  {
    if(children.at(i)->getElementType() != XCLSyntaxElement::PROCESS_TYPE)
      ((XCLSyntaxExpression*)(children.at(i)))->setIsBigEndian(getIsBigEndian());
  }
}


void XCLSyntaxItem::setIsBigEndian(BOOL bo)
{
  XCLSyntaxExpression::setIsBigEndian(bo);
  _LENGTH size = children.size();
  for(_LENGTH i=0; i<size;i++)
  {
    if(children.at(i)->getElementType() != XCLSyntaxElement::PROCESS_TYPE)
      ((XCLSyntaxExpression*)(children.at(i)))->setIsBigEndian(getIsBigEndian());
  }
}


void XCLSyntaxItem::generateIDs(QString ip,_UINT32 iN)
{
  _UINT32 symbolNum=1;
  _UINT32 propertyNum=1;
  _UINT32 itemNum=1;
  QString itemPrefix;
  itemPrefix.setNum(iN);
  itemPrefix.prepend("i");
  itemPrefix.prepend(ip+"_");
  QString symbolPrefix = itemPrefix+"_s";
  QString propertyPrefix = itemPrefix+"_p";

  setIdentifier(itemPrefix);

  QVector<XCLSyntaxElement*>::iterator pos;
  for(pos=children.begin();pos < children.end();++pos)
  {
    XCLSyntaxElement* expr= (*pos);
    if(expr->getElementType() == XCLSyntaxExpression::ITEM_TYPE)
    {
      ((XCLSyntaxItem*)expr)->generateIDs(itemPrefix,itemNum++);
    }
    else if(expr->getElementType() == XCLSyntaxExpression::SYMBOL_TYPE)
    {
      QString newID;
      newID.setNum(symbolNum++);
      newID.prepend(symbolPrefix);
      ((XCLSyntaxSymbol*)expr)->setIdentifier(newID);
    }
    else if(expr->getElementType() == XCLSyntaxExpression::PROPERTY_TYPE)
    {
      QString newID;
      newID.setNum(propertyNum++);
      newID.prepend(propertyPrefix);
      ((XCLSyntaxProperty*)expr)->setIdentifier(newID);
    }

  }

}


void XCLSyntaxItem::generateIDs()
{
  generateIDs("i",1);
}


/*! \void XCLSyntaxItem::addChildsToGrammarIndex(QHash<QString,XCLSyntaxExpression*>& index)
 *  \brief Adds all children of the item to a hash table.
 *  \param index A Hash table holding XCLSyntaxExpressions and their ID-Strings.
 
 * Every element which is not of the type of a processing instruction is added to the table.\n
 * For every item element a Hash is created that contains its own child elements.
 */
void XCLSyntaxItem::addChildsToGrammarIndex(QHash<QString,XCLSyntaxExpression*>& index)
{
  int size=children.size();
  for(int i=0;i<size;++i)
  {
    if(children.at(i)->getElementType() != PROCESS_TYPE)
    {
      XCLSyntaxExpression* expr= (XCLSyntaxExpression*)children.at(i);
      index.insert(expr->getIdentifier(),expr);
      if((children.at(i))->getElementType() == XCLSyntaxExpression::ITEM_TYPE)
      {
        ((XCLSyntaxItem*)(children.at(i)))->addChildsToGrammarIndex(index);
      }
    }
  }
}


