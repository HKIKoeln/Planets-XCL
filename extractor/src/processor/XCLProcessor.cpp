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
#include "XCLProcessor.h"
#include "XCLStringConverter.h"
#include "Defines.h"
#include "XCLDataConverter.h"
#include "XCLCalculator.h"
#include "XCLFilter.h"
//#include "XCELFileParser.h"
#include "XCLInputNormalizer.h"
XCLProcessor::XCLProcessor()
{}


XCLProcessor::~XCLProcessor()
{}
/*! \void XCLProcessor::process(XCLSyntaxExpression* expr, XCLParsingItem* item, const QVector<XCLProcessMethod>& methods, FileParserState&& state)
 *  \brief Called by the Factory class to manage the processing of methods performed on an existing XCLSyntaxEpression.
 *  \param expr A pointer to the XCLSyntaxExpression, the processing method will be performed on.
 *  \param item A pointer to the parent XCLParsingItem.
 *  \param methods A list of the methods which have to be executed on the XCLSyntaxExpression expr.
 *  \param actState A reference to the current FileParserState&.
 *  \exception XCLException 

Calls for each method found in the list the appropriate function to perform the particular task.
**/
void XCLProcessor::process(
    XCLSyntaxExpression* expr,
    FileParserState& item,
    const QVector<XCLProcessMethod>& methods)
{
 // std::cout<<"Processor process\n";
  QVector<XCLProcessMethod> m=methods;
  _LENGTH size = m.size();
  for (_LENGTH i=0;i<size;i++)
  {
    QString name=(m[i].getName());
    if(name == "setLength")
    {
      //std::cout<<"Processor process setLength\n";
      performSetLength(expr,m[i].getParameters(),item);
    }
    else if(name == "setName")
    {
   //   std::cout<<"Processor process setName\n";
      performSetName(expr,m[i].getParameters(),item);
    }
    else if(name == "setByteOrder")
    {
   //   std::cout<<"Processor process setByteOrder\n";
      performSetByteOrder(expr,m[i].getParameters(),item);
    }
    else if(name == "setStartPosition")
    {
    //  std::cout<<"Processor process setStartPosition\n";
      performSetStart(expr,m[i].getParameters(),item);
    }
    else if(name == "setInterpretation")
    {
    //  std::cout<<"Processor process setStartPosition\n";
//      performSetInterpretation(expr,m[i].getParameters(),item);
    }
    else if(name == "setOptional")
    {
    //  std::cout<<"Processor process setStartPosition\n";
//      performSetOptional(expr,m[i].getParameters(),item);
    }
    else if(name == "addFilter")
    {
        performAddFilter(expr,m[i].getParameters(),item);
        
    }
  
    /*else if(name == "addSemanticInterpretation")
    		{std::cout<<"Processor process setByteOrder\n";
    		performSetByteOrder(expr,m[i].getParameters(),item);
    		}
    else if(name == "setValue")
    		{std::cout<<"Processor process setByteOrder\n";
    		performSetByteOrder(expr,m[i].getParameters(),item);
    		}*/
  }
}

void XCLProcessor::process( 	QByteArray* ba,
               			FileParserState& state,
               			const QVector<XCLProcessMethod>& methods)
{
  QVector<XCLProcessMethod> m=methods;
  _LENGTH size = m.size();
  for (_LENGTH i=0;i<size;i++)
  {
    QString name=(m[i].getName());
    if(name == "decompress")
    {
	performDecompress(ba,m[i].getParameters(),state);
    }
 }
}
/*! \void XCLProcessor::performSetLength(XCLSyntaxExpression* expr, QVector<XCLProcessParameter> param,XCLParsingItem* item)
 *  \brief Sets the length of the XCLSyntaxExpression \a expr to the value given by the parameters of the method.
 *  \param expr A pointer to the XCLSyntaxExpression, the processing method will be performed on.
 *  \param param A list of parameters for this method.
 *  \param item A pointer to the parent XCLParsingItem.
 *  \exception XCLException 


**/
void XCLProcessor::performSetLength(XCLSyntaxExpression* expr, QVector<XCLProcessParameter> param,FileParserState& item)
{
    _UINT32 count=0;
    QString count2; // referenced or calculated value as QString

    BOOL isBigEndian;
    QString interpretation;
try
{
    //Value and DataType is given   
    if(param.size()==2)
    {
        XCLProcessParameter p1=param.at(0); //the value
        XCLProcessParameter p2=param.at(1); //the data type

        QString type = p2.getValue(item.index);

        //value has to be calculated
        if (p1.getValueType() == XCLProcessParameter::MATHEX)
        {
        
            XCLCalculator calc;
            count2 = calc.parseExpression(p1.getValue(), item.index);
           
        //    count=XCLStringConverter::string2Number<_UINT32>(count2,"uint64",expr->getIsBigEndian()); //something funny returns here ???
            count=count2.toInt();
        }
        else
        {   
            XCLInputNormalizer normalizer;
            isBigEndian = (item.index.get(p1.getValueReference()))->at(0)->getIsBigEndian();
            interpretation = (item.index.get(p1.getValueReference()))->at(0)->getInterpretation();

            if (isBigEndian)
            {
       

		QByteArray ba = p1.getValueAsBA(&item.index);

		count = (normalizer.normalizeValue((UCHAR*)ba.data(),ba.size(),interpretation, isBigEndian)->toInt());
                //count = p1.getValue(item.index).toLong();
            }
            else
            {
         

		QByteArray ba = p1.getValueAsBA(&item.index);

		count = (normalizer.normalizeValue((UCHAR*)ba.data(),ba.size(),interpretation, isBigEndian)->toInt());

            }            
        }

        _UINT8 typeLength = getTypeLength(type);
     

        expr->setLength(count*typeLength);
     

    }
    else if (param.size()==1)
    {
        XCLProcessParameter p=param.at(0);
        
        //value has to be calculated
        if (p.getValueType() == XCLProcessParameter::MATHEX)
        {
            XCLCalculator calc;
            count = calc.parseExpression(p.getValue(), item.index);
       
            expr->setLength(count);
        }
        else
        {
            _UINT32 num1=p.getValue(item.index).toLong();
      
            expr->setLength(num1); 
    
        }

    }
    else
    {
throw XCLException("Possible candidates for setLength are: setLength( length ) or setLength( count , type )\n");

    }
}
catch(XCLException exception)
{
exception.message();
throw XCLException("XCLProcessor couln´t execute setLength()\n");
}
}
    

/*! \void XCLProcessor::performSetName(XCLSyntaxExpression* expr, QVector<XCLProcessParameter> param,XCLParsingItem* item)
 *  \brief Sets the name of the XCLSyntaxExpression \a expr to the value given by the parameter of the method.
 *  \param expr A pointer to the XCLSyntaxExpression, the processing method will be performed on.
 *  \param param A list of parameters for this method.
 *  \param item A pointer to the parent XCLParsingItem.
 *  \exception XCLException 


**/

void XCLProcessor::performSetName(XCLSyntaxExpression* expr, QVector<XCLProcessParameter> param,FileParserState& item)
{
  XCLProcessParameter p=param.at(0);

  expr->setName(p.getValue(item.index));
}

/*! \void XCLProcessor::performSetByteOrder(XCLSyntaxExpression* expr, QVector<XCLProcessParameter> param,XCLParsingItem* item)
 *  \brief Sets the byteorder of the XCLSyntaxExpression \a expr to the value given by the parameter of the method.
 *  \param expr A pointer to the XCLSyntaxExpression, the processing method will be performed on.
 *  \param param A list of parameters for this method.
 *  \param item A pointer to the parent XCLParsingItem.
 *  \exception XCLException 


**/
void XCLProcessor::performSetByteOrder(XCLSyntaxExpression* expr, QVector<XCLProcessParameter> param,FileParserState& item)
{
    XCLProcessParameter p=param.at(0);
  

    expr->setByteOrder((p.getValue(item.index)));
}




/*! \void XCLProcessor::performSetFilter(XCLSyntaxExpression* expr, QVector<XCLProcessPanrameter> param,XCLParsingItem* item)
 *  \brief 
 *  \param expr A pointer to the XCLSyntaxExpression, the processing method will be performed on.
 *  \param param A list of parameters for this method.
 *  \param item A pointer to the parent XCLParsingItem.
 *  \exception XCLException 


**/
void XCLProcessor::performAddFilter(XCLSyntaxExpression* expr,QVector<XCLProcessParameter> param,FileParserState& item)
{

    XCLFilter* filter = new XCLFilter;

    //the filtername (i.e. compression, interlace..) is conteined by the value of first parameter
    XCLProcessParameter filtername=param.at(0);
    filter->setFilterName(filtername.getValue(item.index));


    //the filtervalue (i.e. zLibDeflateInflate, adam7..) is rerferenced by the second parameter
    XCLProcessParameter filtervalue=param.at(1);
    filter->setFilterValue(filtervalue.getValue(item.index));


    // if the filter depends on further parameters the information is added here
    XCLProcessParameter filterparam;
    for (int i=2;i<param.size();i++)
    {
        filterparam = param.at(i);
        filter->addParam(filterparam.getNameReference(), filterparam.getValue(item.index));
    }
    expr->setFilter(*filter);
}


/*! \void XCLProcessor::performSetStart(XCLSyntaxExpression* expr, QVector<XCLProcessParameter> param,XCLParsingItem* item)
 *  \brief Sets the startposition of the XCLSyntaxExpression \a expr to the value given by the parameter of the method.
 *  \param expr A pointer to the XCLSyntaxExpression, the processing method will be performed on.
 *  \param param A list of parameters for this method.
 *  \param item A pointer to the parent XCLParsingItem.
 *  \param state The current FileParserState&.
 *  \exception XCLException 

**/
void XCLProcessor::performSetStart(XCLSyntaxExpression* expr,
                                   QVector<XCLProcessParameter> param,
                                   FileParserState& item)
{

    XCLProcessParameter p=param.at(0);

    _UINT32 num;
    QString numstring;
 
 

    if (p.getValueReference().startsWith("#"))
    {
     
        XCLCalculator calc;
        numstring = calc.parseExpression(p.getValueReference(), item.index);
    
    }

    if(p.getListRef())
    {
        IndexedVector* iv;
        if(vectorHash.contains(p.getValueReference()))
        {
            iv = vectorHash.value(p.getValueReference());
        }
        else
	{
           iv = new IndexedVector();
	   iv->vector = item.index.get(p.getValueReference());
           iv->index=0;
	   vectorHash.insert( p.getValueReference(),iv);
	}
        if((_SINT32)iv->index >= iv->vector->size())
        {
	  throw XCLOutOfRangeException();
	}
        num = iv->vector->at(iv->index++)->getInterpretedValue().toInt();
    }
    else
    { 
        num=p.getValue(item.index).toInt(); 
    }

    expr->setStart(num);
    expr->setPositionType( XCLSyntaxExpression::FIXED );
}




 //expr=new XCLSyntaxItem();


_UINT8 XCLProcessor::getTypeLength(const QString& type)
{
  if(type == "uint8")
  {
    return 1;
  }
  else if(type == "uint16")
  {
    return 2;
  }
  else if(type == "uint32")
  {
    return 4;
  }
  else if(type == "uint64")
  {
    return 8;
  }
  else if(type == "sint8")
  {
    return 1;
  }
  else if(type == "sint16")
  {
    return 2;
  }
  else if(type == "sint32")
  {
    return 4;
  }
  else if(type == "sint64")
  {
    return 8;
  }
  else
  {
    std::cout<<"\""<<type.toStdString()<<"\" is unkown type\n";
    return 1;
  }
}


void XCLProcessor::performDecompress(QByteArray* ba,
			 QVector<XCLProcessParameter> param,
                         FileParserState& state)
{
   //TODO: check parameters !!
   //Test code for zlibDeflateInflate 
    XCLProcessParameter p=param.at(0);
    QString compressionMethod =p.getValue(state.index);
    p=param.at(1);
    QString startStr = p.getValue(state.index);
    p=param.at(2);
    QString endStr = p.getValue( state.index); 
 
    _LENGTH start= startStr.toLong( );
    _LENGTH end= endStr.toLong();
 
    QByteArray head = ba->left(start);
    QByteArray body = ba->mid(start,end);
    QByteArray tail = ba->mid(end,ba->size()); 

    XCLDataConverter convert;

    QVector<XCLFilter> filterChain;
    XCLFilter filter;
    filter.setFilterName("compression");
    filter.setFilterValue("zlibDeflateInflate"); 
    filterChain.append(filter);
    QByteArray cvalue=convert.filter(&body, filterChain);
    delete ba;
    ba =new QByteArray();
    *ba+=head;
    *ba+=body;
    *ba+=tail;
}
