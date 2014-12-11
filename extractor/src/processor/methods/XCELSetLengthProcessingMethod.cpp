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
#include "XCELSetLengthProcessingMethod.h"
#include "Defines.h"
#include "XCLCalculator.h"

BOOL* XCELSetLengthProcessingMethod::getMode()
{
  BOOL mod[]={FALSE,TRUE,FALSE,FALSE};
  return mod;
}




int XCELSetLengthProcessingMethod::expectedParameters()
{
  return 2;
}


BOOL XCELSetLengthProcessingMethod::preprocess(XCELReader* reader, XCLSyntaxItem& grammar, const XCLSyntaxProcess& preprocessing)
{
  return FALSE;
  // useless statements for less compiler-warnings, params are in use
  (void)reader;
  (void)grammar;
  (void)preprocessing;
}


BOOL XCELSetLengthProcessingMethod::process(	XCELReader* reader,
			XCLSyntaxExpression* expr,
                        QVector<XCLProcessParameter> param,
                        QHash<QString,IndexedVector*> vectorHash)
{
  _UINT32 count=0;
  QString count2; // referenced or calculated value as QString
  FileParserState* state = reader->getState();
  BOOL isBigEndian;
  QString interpretation;
  try
  {
    //Value and DataType is given
    if(param.size()==2)
    {
      XCLProcessParameter p1=param.at(0); //the value
      XCLProcessParameter p2=param.at(1); //the data type

      QString type = p2.getValue(state->index);

      //value has to be calculated
      if (p1.getValueType() == XCLProcessParameter::MATHEX)
      {

        XCLCalculator calc;
        count2 = calc.parseExpression(p1.getValue(), state->index);

        //    count=XCLStringConverter::string2Number<_UINT32>(count2,"uint64",expr->getIsBigEndian()); //something funny returns here ???
        count=count2.toInt();
      }
      else
      {
        XCLInputNormalizer normalizer;
        isBigEndian = (state->index.get(p1.getValueReference()))->at(0)->getIsBigEndian();
        interpretation = (state->index.get(p1.getValueReference()))->at(0)->getInterpretation();

       /* if (isBigEndian)
        {*/


          QByteArray ba = p1.getValueAsBA(&state->index);

          count = (normalizer.normalizeValue((UCHAR*)ba.data(),ba.size(),interpretation, isBigEndian)->toInt());
          //count = p1.getValue(item.index).toLong();
      /*  }
        else
        {


          QByteArray ba = p1.getValueAsBA(state.index);

          count = (normalizer.normalizeValue((UCHAR*)ba.data(),ba.size(),interpretation, isBigEndian)->toInt());

        }*/
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
        count = calc.parseExpression(p.getValue(), state->index);

        expr->setLength(count);
      }
      else
      {
        _UINT32 num1=p.getValue(state->index).toLong();

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
    throw XCLException("XCELMainProcessor couln´t execute setLength()\n");
  }

  // ### Wozu an dieser Stelle?
  // QByteArray* ba=reader->getBinaryArray();

  expr->setLengthType(XCLSyntaxExpression::FIXED);  
  return TRUE;
  // useless statements for less compiler-warnings, params are in use
  (void)vectorHash;
}




BOOL XCELSetLengthProcessingMethod::postprocess( XCLParsingItem& parseTree,
							QVector<XCLProcessParameter> param,
							QVector<XCLParsingExpression*> referenceData,
							FileParserState& state,
							QHash<QString,IndexedVector*> vectorHash)
{
  return FALSE;
  // useless statements for less compiler-warnings, params are in use
  (void)parseTree;
  (void)param;
  (void)referenceData;
  (void)state;
  (void)vectorHash;
}

_UINT8 XCELSetLengthProcessingMethod::getTypeLength(const QString& type)
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
