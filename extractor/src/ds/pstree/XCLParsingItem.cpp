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
#include "XCLParsingItem.h"
#include "XCLParsingSymbol.h"
#include "XCLParsingProperty.h"
#include "XCLDataConverter.h"
#include "XCLInputNormalizer.h"
#include "XCLParsingExpression.h"


XCLParsingItem::XCLParsingItem ():XCLParsingExpression()
{
  children.clear();
  objectType="other";
}


XCLParsingItem::XCLParsingItem (const XCLParsingItem& itm)
    :XCLParsingExpression(itm)
{
  setChildren(itm.children);
  objectType=itm.objectType;
}


XCLParsingItem::~XCLParsingItem ( )
{
}


XCLParsingItem& XCLParsingItem::operator=(const XCLParsingItem& itm)
{
  if(this!=&itm)
  {
    XCLParsingExpression::operator=(itm);
    setChildren(itm.children);
    objectType=itm.objectType;
  }
  return *this;
}

 void XCLParsingItem::setObjectType(const QString& objT)
{
objectType=objT;
}
  const QString& XCLParsingItem::getObjectType()
{
return objectType;
}
enum XCLParsingExpression::INTRPR_TYPE XCLParsingItem::getInterpretationType()
{
  if(!children.isEmpty())
    return children.at(0)->getInterpretationType();
  else
    throw XCLException("item has no content!");
}

XCLParsingSymbol* XCLParsingItem:: getFirstProperty()
{
	for ( int i=0;i<children.size();i++)
	{
		if(children.at(i)->getFirstProperty() != NULL)
			return (XCLParsingSymbol*)children.at( i);
	}
	return NULL;
}
void XCLParsingItem::removeExpressions(const QString& name)
{
  QString n=name;
  for(_SIZE i=0; i<children.size(); i++)
  {
    XCLParsingExpression* expr= children.at(i);
    if(expr->getType() == XCLParsingExpression::ITEM_TYPE)
    {
      (((XCLParsingItem*)expr)->removeExpressions(name));
    }
    if(expr->getName()==name)
    {
      children.remove(i);
    }
  }

}


/**
 * Set the value of children
 * @param new_var the new value of children
 */
void XCLParsingItem::setChildren ( const QVector<XCLParsingExpression*>& es )
{
  children.clear();
  addChildren(es);
}


/**
 * Get the value of children
 * @return the value of children
 */
const QVector<XCLParsingExpression*>& XCLParsingItem::getChildren ( )
{
  return children;
}


/**
 * @return _UINT32
 */
_SIZE XCLParsingItem::getLength ( )
{
  _LENGTH result=0;
  QVector<XCLParsingExpression*>::iterator pos;
  for(pos = children.begin();pos<children.end();++pos)
  {
    result += (*pos)->getLength();
  }
  return result;
}



/**
 * @return QString
 */
const QByteArray& XCLParsingItem::getValue ( )
{
  QByteArray* value = new QByteArray();
  QVector<XCLParsingExpression*>::iterator pos;
  for(pos=children.begin();pos < children.end();++pos)
  {

    XCLParsingExpression* expr= (*pos);
 
    value->append(expr->getValue());
  }

  return *value;
}


/**
 * @return QString; returns the interpreted values of all of its children
 */
const QString& XCLParsingItem::getInterpretedValue ( )
{
  QString* parsedValue = new QString();
  if(!filterChain.isEmpty())
  {

//std::cout<<"Enter Filter processing...\n";
    XCLDataConverter convert;
    QByteArray value = getValue();
//std::cout<<value.data()<<"\n";
    QByteArray cvalue=convert.filter(&value, filterChain);
    XCLInputNormalizer normalizer;
    parsedValue = normalizer.normalizeValue( (UCHAR*)(cvalue.data()),cvalue.size(),getInterpretation(),getIsBigEndian());

    return *parsedValue;
  }
  else
  {
    INTRPR_TYPE interpretation = getInterpretationType();
    QVector<XCLParsingExpression*>::iterator pos;
    for(pos=children.begin();pos < children.end();++pos)
    {
      XCLParsingExpression* expr= (*pos);
      parsedValue->append(expr->getInterpretedValue());
      if(interpretation != CHARACTER)
        parsedValue->append(" ");
    }
    if(interpretation != CHARACTER)
      parsedValue->remove(parsedValue->size()-1,1);
    return *parsedValue;
  }
}


const QString& XCLParsingItem::getInterpretation ( )
{
  if(children.isEmpty())
    throw XCLException("Interpretation of item without children is UNKNOWN\n");
  else
    return children.first()->getInterpretation();
}

XCLParsingExpression* XCLParsingItem::getFirstChild()
{
return children.first();
}
XCLParsingExpression* XCLParsingItem::getLastChild()
{
  XCLParsingExpression* expr=children.last();
  enum EXPR_TYPE exprType=expr->getType();
  if(exprType == XCLParsingExpression::ITEM_TYPE)
  {
    return new XCLParsingItem(*((XCLParsingItem*)expr));
  }
  else if(exprType == XCLParsingExpression::SYMBOL_TYPE)
  {
    return new XCLParsingSymbol(*((XCLParsingSymbol*)expr));
  }
  else if(exprType == XCLParsingExpression::PROPERTY_TYPE)
  {
    return new XCLParsingProperty(*((XCLParsingProperty*)expr));
  }

  return NULL;
}


enum XCLParsingItem::EXPR_TYPE XCLParsingItem::getType()
{
  return XCLParsingExpression::ITEM_TYPE;
}


void XCLParsingItem::add(XCLParsingExpression* expr)
{

  enum XCLParsingExpression::EXPR_TYPE type=expr->getType();

  if(type == ITEM_TYPE)
  {

    XCLParsingItem* item=new XCLParsingItem(*((XCLParsingItem*)expr));

    item->setParent(this);
    children.append(item);

  }
  else if(type == PROPERTY_TYPE)
  {
    children.append(new XCLParsingProperty(*((XCLParsingProperty*)expr)));
    children.last()->setParent(this);

  }
  else if(type == SYMBOL_TYPE)
  {
    children.append(new XCLParsingSymbol(*((XCLParsingSymbol*)expr)));
    children.last()->setParent(this);
  }

}


void XCLParsingItem::generateIDs()
{
  QHash<QString,QString>* idMap = new QHash<QString,QString>();
  generateIDs(this,"i",1,idMap);
  delete idMap;
}


void XCLParsingItem::generateIDs(XCLParsingItem* root, QString ip,_UINT32 iN,QHash<QString,QString>* idMap)
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

  idMap->insertMulti(getIdentifier(),itemPrefix);
  setIdentifier(itemPrefix);

  QVector<XCLParsingExpression*>::iterator pos;
  for(pos=children.begin();pos < children.end();++pos)
  {
    XCLParsingExpression* expr= (*pos);
    if(expr->getType() == XCLParsingExpression::ITEM_TYPE)
    {
      ((XCLParsingItem*)expr)->generateIDs(root,itemPrefix,itemNum++,idMap);
    }
    else if(expr->getType() == XCLParsingExpression::SYMBOL_TYPE)
    {

      QString newID;
      newID.setNum(symbolNum++);
      newID.prepend(symbolPrefix);
      idMap->insertMulti(expr->getIdentifier(),newID);
      ((XCLParsingSymbol*)expr)->setIdentifier(newID);

    }
    else if(expr->getType() == XCLParsingExpression::PROPERTY_TYPE)
    {

      QString newID;
      newID.setNum(propertyNum++);
      newID.prepend(propertyPrefix);
      idMap->insertMulti(expr->getIdentifier(),newID);
      ((XCLParsingProperty*)expr)->setIdentifier(newID);
    }

    QVector<XCLParsingExpression*> rel = expr->getRelations();

    if(!rel.isEmpty())
    {	
        //std::cout<<expr->getIdentifier().toStdString()<<" SET RELATIONS\n";
	QVector<XCLParsingExpression*> newRel;
	for(int i=0;i<rel.size();i++)
	{
	  if(idMap->contains(rel.at(i)->getIdentifier()))
	  {
		QList<QString> newIds = idMap->values(rel.at(i)->getIdentifier());
		for(int j=0;j<newIds.size();j++)
		{
			QString actId = newIds.at(j);
			XCLParsingExpression* expr2 = root->getChild(actId);
			if(expr2->getInterpretedValue() == rel.at(i)->getInterpretedValue())
			{
				
				newRel.append(expr2);
				break;
			}
		}
 	  }	
	}
	expr->setRelations(newRel);
    }
  }
}


void XCLParsingItem::addChildren(const QVector<XCLParsingExpression*>& ch)
{
  QVector<XCLParsingExpression*> childs =ch;
  QVector<XCLParsingExpression*>::iterator pos;
  for(pos = childs.begin();pos<childs.end();++pos)
  {
    add(*pos);
  }
}


XCLParsingExpression* XCLParsingItem::getChild(QString id)
{
  if(getIdentifier() == id)
	return this;
  XCLParsingExpression* result=NULL;
  QVector<XCLParsingExpression*>::iterator pos;
  for(pos=children.begin();pos < children.end();++pos) //loop over all children
  {
    XCLParsingExpression* expr= (*pos);
    if(expr->getIdentifier() == id)
	return expr;
    // std::cout<<"Insert "<<((XCLParsingItem*)expr)->getName().toStdString()<<"\n";
    if(expr->getType() == XCLParsingExpression::ITEM_TYPE)//if child is an item, it might have children itself...
    {
      result=((XCLParsingItem*)expr)->getChild(id);
      if(result != NULL)
	return result;
    }
  }
	return result;
}
const QVector<XCLParsingExpression*>& XCLParsingItem::getChildren(QString id)
{
 QVector<XCLParsingExpression*> result;
  if(getIdentifier() == id)
	result.append(this);
  
  QVector<XCLParsingExpression*>::iterator pos;
  for(pos=children.begin();pos < children.end();++pos) //loop over all children
  {
    XCLParsingExpression* expr= (*pos);
    
    // std::cout<<"Insert "<<((XCLParsingItem*)expr)->getName().toStdString()<<"\n";
    if(expr->getType() == XCLParsingExpression::ITEM_TYPE) //if child is an item, it might have children itself...
    {
      result+=((XCLParsingItem*)expr)->getChildren(id);
    }
    else if (expr->getIdentifier() == id)
	result.append(expr);
  }
  return result;
}

// return: enthält alle ParsingExpressions, die das Attribut newObject==true haben
//es werden nur die Objekte zurückgeliefert, die unmittelbare Kinder von this sind.
const QVector<XCLParsingExpression*>&  XCLParsingItem::getObjectsLevel1()
{
    QVector<XCLParsingExpression*> * result  = new QVector<XCLParsingExpression*>();
  	
//     std::cout<<"\n----***-in XCLParsingItem::getObjectsLevel1() --***---\n\n";

    QVector<XCLParsingExpression*>::iterator pos;
    for(pos=children.begin();pos < children.end();++pos) //loop over all children
    {
        try{

            XCLParsingExpression* expr= (*pos);
            if( expr->getNewObject()){

                XCLParsingItem *pI =new XCLParsingItem(*(XCLParsingItem*)(expr));
                result->append(pI);
//                      std::cout<<"\n "<<pos<<"="<< expr->getIdentifier().toStdString();
//                      std::cout<<"\n"<< expr->getName().toStdString();
            }

        } catch(XCLException exception) 
        {
        }
    }
   
    return *result;
}

// Parse Tree kann mehrere Objekte enthalten.
//Diese Methode liefert als liste alle Objecte des Parstrees zurück.
        const QVector<XCLParsingExpression*>&  XCLParsingItem::pickObjects()
{
    QVector<XCLParsingExpression*>* o2 =  new QVector<XCLParsingExpression*>();
 
  	
   

    QVector<XCLParsingExpression*>::iterator pos;
    for(pos=children.begin();pos < children.end();++pos) //loop over all children
    {
        try{

            XCLParsingExpression* expr= (*pos);
            if( expr->getNewObject())
            {
//                 std::cout<<"\n----***-in XCLParsingItem::pickObjects() --***---\n";
                XCLParsingItem *pI =new XCLParsingItem(*(XCLParsingItem*)(expr));
                o2->append(pI);
//                      std::cout<<"\n "<<pos<<"="<< expr->getIdentifier().toStdString();
//                      std::cout<<"\n"<< expr->getName().toStdString()<<"\n\n";
            }
            if(expr->getType() == XCLParsingExpression::ITEM_TYPE) //if child is an item, it might have children itself...
            {
//                 std::cout<<"\n "<<pos<<"="<< expr->getIdentifier().toStdString();
//                 std::cout<<"\n"<< expr->getName().toStdString()<<"\n\n";
                if(!( (XCLParsingItem*)expr)->children.isEmpty())
                    *o2 +=  ( (XCLParsingItem*) expr)->pickObjects();
			
            }
            
            
            

        } catch(XCLException exception) 
        {
        }
    }
//     std::cout<<"\n----***- END in XCLParsingItem::pickObjects() --***---\n\n";
    return *o2;
}

const QHash<QString,XCLParsingExpression*>& XCLParsingItem::getChildsByName2()
{
    
    QHash<QString,XCLParsingExpression*>* theExpressions = new QHash<QString,XCLParsingExpression*>();
//     std::cout<<"\ngetChildrenByName2:::\n";
    QString currentName=getName();
    if(!currentName.isEmpty())
        theExpressions->insertMulti(getName(),this); //inserts a String and an XCLParsingExpression, ie. the name and the expression object which called this method; does not overwrite existing items with same name
    QVector<XCLParsingExpression*>::iterator pos;
    for(pos=children.begin();pos < children.end();++pos) //loop over all children
    {
        XCLParsingExpression* expr= (*pos);
    // std::cout<<"Insert "<<((XCLParsingItem*)expr)->getName().toStdString()<<"\n";
        if(expr->getType() == XCLParsingExpression::ITEM_TYPE && !expr->getNewObject()) 
//if child is an item, it might have children itself...
//stop, if child is an object.Seine Properties werden unter diesem Object vervaltet.
    	{
		QHashIterator<QString, XCLParsingExpression*> j(((XCLParsingItem*)expr)->getChildsByName2());
		while (j.hasNext())
		{
			j.next();
			theExpressions->insertMulti(j.key(),j.value());
		}
	}
        else{
            currentName=expr->getName();
            if(!currentName.isEmpty())//Der PropertyName von object ist Property von Mainobject und nicht von sich selbt
                theExpressions->insertMulti(expr->getName(),expr);
        }

    }
    return *theExpressions;
}

/*! \const QHash<QString,XCLParsingExpression*>& XCLParsingItem::getChildsByName()
 *  \brief Iterates over all children of the oject calling and inserts them into a hash table accessible by their name as QString.
 
 * First, this function inserts a String and an XCLParsingExpression, ie. the name and the expression object which called this method; does not overwrite existing items with same name \n
 * Then, it loops over all children \n
 * if child is an item, it might have children itself -> second loop \n
 
 *  \return A hash table that contains all child elements of the XCLParsing Expression which called this method accessible their names as QString.
 */
const QHash<QString,XCLParsingExpression*>& XCLParsingItem::getChildsByName()
{
  
  QHash<QString,XCLParsingExpression*>* theExpressions = new QHash<QString,XCLParsingExpression*>();
  
  QString currentName=getName();
  if(!currentName.isEmpty()&&!getNewObject())//Der PropertyName von object ist Property von Mainobject und nicht von sich selbt
      theExpressions->insertMulti(getName(),this); //inserts a String and an XCLParsingExpression, ie. the name and the expression object which called this method; does not overwrite existing items with same name
  QVector<XCLParsingExpression*>::iterator pos;
  for(pos=children.begin();pos < children.end();++pos) //loop over all children
  {
    XCLParsingExpression* expr= (*pos);
    // std::cout<<"Insert "<<((XCLParsingItem*)expr)->getName().toStdString()<<"\n";
    if(expr->getType() == XCLParsingExpression::ITEM_TYPE) //if child is an item, it might have children itself...
    {
      QHashIterator<QString, XCLParsingExpression*> j(((XCLParsingItem*)expr)->getChildsByName());
      while (j.hasNext())
      {
        j.next();
        currentName=j.key();
        if(!currentName.isEmpty()&&!expr->getNewObject())//Der PropertyName von object ist Property von Mainobject und nicht von sich selbt
            theExpressions->insertMulti(j.key(),j.value());
      }
    }
    else //just insert symbol or property
    {
        currentName=expr->getName();
        if(!currentName.isEmpty()&&!expr->getNewObject())//Der PropertyName von object ist Property von Mainobject und nicht von sich selbt
            theExpressions->insertMulti(expr->getName(),expr);
    }

  }
  return *theExpressions;
}


const QVector<XCLParsingExpression*>& XCLParsingItem::getOrderedData(const QString& name)
{
  QString n=name;
  QVector<XCLParsingExpression*>* list = new QVector<XCLParsingExpression*>();
  QVector<XCLParsingExpression*>::iterator pos;
  for(pos=children.begin();pos < children.end();++pos) //loop over all children
  {
    try
    {
      XCLParsingExpression* expr= (*pos);
      if(expr->getName()==name)
      {
        XCLParsingExpression::EXPR_TYPE type=expr->getType();
        if(type == XCLParsingExpression::ITEM_TYPE)
        {
          list->append(new XCLParsingItem(*(XCLParsingItem*)(expr)));
        }
        else if(type == XCLParsingExpression::SYMBOL_TYPE)
        {
          list->append(new XCLParsingSymbol(*(XCLParsingSymbol*)(expr)));

        }
        else if(type == XCLParsingExpression::PROPERTY_TYPE)
        {
          list->append(new XCLParsingProperty(*(XCLParsingProperty*)(expr)));
        }
      }
      else if(expr->getType() == XCLParsingExpression::ITEM_TYPE)
      {
        *list+=(((XCLParsingItem*)expr)->getOrderedData(name));
      }
    }
    catch(XCLException exception)
    {

    }
  }

  if (list->isEmpty())
    throw XCLException("No Expression with ID: "+name+" found! You might check XCEL for mistakes.");
  return *list;
}

/*
return: Contains all expressions with name normData and all expressions related to the normData
*/
const QVector<XCLParsingExpression*>& XCLParsingItem::getNormData()
{
  QString n="normData";
  QVector<XCLParsingExpression*>* list = new QVector<XCLParsingExpression*>();
  QVector<XCLParsingExpression*>::iterator pos;
  for(pos=children.begin();pos < children.end();++pos) //loop over all children
  {
    try
    {
      XCLParsingExpression* expr= (*pos);
      if( expr->getName()==n||expr->getNormDataRelation() )
      {
            XCLParsingExpression::EXPR_TYPE type=expr->getType();
            if(type == XCLParsingExpression::ITEM_TYPE)
            {
            list->append(new XCLParsingItem(*(XCLParsingItem*)(expr)));
            }
            else if(type == XCLParsingExpression::SYMBOL_TYPE)
            {
            list->append(new XCLParsingSymbol(*(XCLParsingSymbol*)(expr)));
            }
            else if(type == XCLParsingExpression::PROPERTY_TYPE)
            {
            list->append(new XCLParsingProperty(*(XCLParsingProperty*)(expr)));
            }

      }
      else if(expr->getType() == XCLParsingExpression::ITEM_TYPE  && !expr->getNewObject() )
      {
            *list+=(((XCLParsingItem*)expr)->getNormData());
      }
    }
    catch(XCLException exception)
    {

    }
  }

  if (list->isEmpty())
    throw XCLException("No Expression with ID: "+name+" found! You might check XCEL for mistakes.");
  return *list;
}

XCLTree<XCLParsingExpression*>* XCLParsingItem::getNameIndex()
{
  XCLTree<XCLParsingExpression*>* treeIndex = new XCLTree<XCLParsingExpression*>();
  QVector<XCLParsingExpression*>::iterator pos;
  for(pos=children.begin();pos < children.end();++pos) //loop over all children
  {
    try
    {
      XCLParsingExpression* expr= (*pos);
      treeIndex->insert(expr->getName(), expr);
      if(expr->getType() == XCLParsingExpression::ITEM_TYPE)
      {
        treeIndex->insert(((XCLParsingItem*)expr)->getNameIndex());
      }
    }
    catch(XCLException exception)
    {
#ifdef PlanetsDebug
      std::cout<<"Error while creating ParseTree Name-Index. The index might not be complete.\n";
#endif
#ifdef PlanetsDebug
      exception.message();
#endif

    }
  }
  return treeIndex;
}


XCLTree<XCLParsingExpression*>* XCLParsingItem::getIDIndex()
{
  XCLTree<XCLParsingExpression*>* treeIndex = new XCLTree<XCLParsingExpression*>();
  _UINT32 size = children.size();
  if(size==0)throw XCLException("XCLParsingItem:: Item without children. Can not add to treeIndex: "+getIdentifier()+"\n");
  for(_UINT32 i=0;i<size;i++) //loop over all children
  {
    try
    {
      XCLParsingExpression* expr= children.at(i);
      treeIndex->insert(expr->getIdentifier(), expr);
      if(expr->getType() == XCLParsingExpression::ITEM_TYPE)
      {
        treeIndex->insert(((XCLParsingItem*)expr)->getIDIndex());
      }
    }
    catch(XCLException exception)
    {
#ifdef PlanetsDebug
      std::cout<<"Error while creating ParseTree ID-Index. The index might not be complete.\n";
#endif
        #ifdef PlanetsDebug
      exception.message();
#endif

    }
  }
  return treeIndex;
}

void XCLParsingItem::setInterpretedValue(const QString& val)
{
	XCLParsingSymbol* symbol = new XCLParsingSymbol();
	symbol->setInterpretation(getInterpretation());
	symbol->setInterpretedValue(val);
        children.clear();
	add(symbol);
}

BOOL XCLParsingItem::containsValue(const QString& val)
{
 if(getInterpretedValue() == val)
	return TRUE;
  QVector<XCLParsingExpression*>::iterator pos;
  for(pos=children.begin();pos < children.end();++pos)
  {
     XCLParsingExpression* expr= (*pos); 
     if(expr->containsValue(val))
     {	
	return TRUE;
     }
  }
  return FALSE;
}

void XCLParsingItem::print(QTextStream& debugOut, QString f)
{
  XCLParsingExpression::print(debugOut,f);
  QVector<XCLParsingExpression*>::iterator pos;
  for(pos=children.begin();pos < children.end();++pos)
  {
    XCLParsingExpression* expr= (*pos);
    if(expr->getType() == XCLParsingExpression::ITEM_TYPE)
    {
      ((XCLParsingItem*)expr)->print(debugOut,f+"\t");
    }
    else if(expr->getType() == XCLParsingExpression::SYMBOL_TYPE)
    {
      ((XCLParsingSymbol*)expr)->print(debugOut,f+"\t");
    }
    else if(expr->getType() == XCLParsingExpression::PROPERTY_TYPE)
    {
      ((XCLParsingProperty*)expr)->print(debugOut,f+"\t");
    }
  }
}


void XCLParsingItem::print(QString f)
{
  QTextStream treeOut(stdout);
  print(treeOut, f);
}
