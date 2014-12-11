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
#include "XCELSetStartProcessingMethod.h"
#include "XCLCalculator.h"
#include "XCELProcessorInterface.h"

BOOL* XCELSetStartProcessingMethod::getMode()
{
  BOOL mod[]= {FALSE,TRUE,FALSE,FALSE};
  return mod;
}


// ### An dieser Stelle stehtl ein int-Rueckgabewert, aber zurzeit wird kein Wert zurueckgegeben.
int XCELSetStartProcessingMethod::expectedParameters()
{
}


BOOL XCELSetStartProcessingMethod::preprocess(	XCELReader* reader, 
						XCLSyntaxItem& grammar, 
						const XCLSyntaxProcess& preprocessing)
{
  return FALSE;
  // useless statements for less compiler-warnings, params are in use
  (void)reader;
  (void)grammar;
  (void)preprocessing;
}


BOOL XCELSetStartProcessingMethod::process(	XCELReader* reader,
						XCLSyntaxExpression* expr,
                        			QVector<XCLProcessParameter> param,
                        			QHash<QString,IndexedVector*> vectorHash)

{

FileParserState* state = reader->getState();

   XCLProcessParameter p=param.at(0);

    _UINT32 num;
    QString numstring;
 


    if (p.getValueReference().startsWith("#"))
    {
   
        XCLCalculator calc;
        numstring = calc.parseExpression(p.getValueReference(), state->index);
     
    }

    if(p.getListRef())
    {
  // std::cout<<"Enter listRef setStart processing\n";
        IndexedVector* iv;
        if(vectorHash.contains(p.getValueReference()))
        {
            iv = vectorHash.value(p.getValueReference());
          
        }
        else
	{
           iv = new IndexedVector();
	   iv->vector = state->index.get(p.getValueReference());
           iv->index=0;
	   vectorHash.insert( p.getValueReference(),iv);
	}
        // ### Dieses Statement ist eigentlich quatsch, weil iv->index niemals kleiner als 0 werden kann.
	// if(iv->index >= iv->vector->size() || iv->index < 0 )
	if((_SIZE)iv->index >= iv->vector->size())
        {
	    throw XCLOutOfRangeException("List Ref contains no more values");
	}
        num = iv->vector->at(iv->index++)->getInterpretedValue().toInt();
#ifdef PlanetsDebug
        std::cout<<"NUM: "<<num<<"\n";
#endif
    }
    /*else if(p.getStackRef())
    {
        IndexedVector* iv;
        if(vectorHash.contains(p.getValueReference()))
        {
            iv = vectorHash.value(p.getValueReference());
          
        }
        else
	{
           iv = new IndexedVector();

	   iv->vector = state->index.get(p.getValueReference());
      
           iv->index=iv->verctor.size()-1;
          
	   vectorHash.insert( p.getValueReference(),iv);
	}
        if(iv->index >= iv->vector->size())throw XCLOutOfRangeException();
        num=iv->vector->at(iv->index--)->getInterpretedValue().toInt();
     }*/
    else
    { 
//p.print();
  
        QString value=p.getValue(state->index); 
	num=value.toInt();


    }
    expr->setStart(num);
    expr->setPositionType( XCLSyntaxExpression::FIXED );

  return TRUE;
}



BOOL XCELSetStartProcessingMethod::postprocess( XCLParsingItem& parseTree,
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
