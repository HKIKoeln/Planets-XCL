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
#ifndef XCLPROCESSOR_H
#define XCLPROCESSOR_H
#include "XCLSyntaxExpression.h"
#include "XCLSyntaxSymbol.h"
#include "XCLParsingItem.h"
#include "XCLProcessMethod.h"
#include "XCLProcessParameter.h"
#include "XCLInputNormalizer.h"
#include "XCELReader.h"

//class FileParserState;


/*! \class XCLProcessor XCLProcessor "XCLProcessor"
 *  \brief A class which executes processing methods on an existing XCLSyntaxExpression.
 
The class has one public function process(), which manages the processing of methods by calling the appropriate private function to perform the particular task.
 
  @author Jan Schnasse <jan.schnasse@uni-koeln.de>
 */
class IndexedVector
{
public:
  _UINT32 index;
  QList<XCLParsingExpression*>* vector;

  void print()
  {
    std::cout<<"Index: "<<index<<"\n";
    /*
    #ifdef PlanetsDebug
    for(int i=0;i<vector->size();i++)
    {
    std::cout<<"Identifier "<<vector->at(i)->getIdentifier().toStdString()<<"\n";
    std::cout<<"Value "<<vector->at(i)->getInterpretedValue().toStdString()<<"\n";
    }
    #endif
    */
  }
};
class XCLProcessor
{
public:
  XCLProcessor();

  ~XCLProcessor();
private://members
  QHash<QString,IndexedVector*> vectorHash;
  XCLInputNormalizer normalizer;
public:
//processing for XCEL operations
  void process(XCLSyntaxExpression* expr,
               FileParserState& item,
               const QVector<XCLProcessMethod>& methods);
//processing for binary operations
 void process( QByteArray* ba,
               FileParserState& state,
               const QVector<XCLProcessMethod>& methods);
 
private:
  void performSetLength(XCLSyntaxExpression* expr,
                        QVector<XCLProcessParameter> param,
                        FileParserState& item);
  void performSetName(XCLSyntaxExpression* expr,
                      QVector<XCLProcessParameter> param,
                      FileParserState& item);
  void performSetByteOrder(XCLSyntaxExpression* expr,
                           QVector<XCLProcessParameter> param,
                           FileParserState& item);
  void performSetStart(XCLSyntaxExpression* expr,
                       QVector<XCLProcessParameter> param,
                       FileParserState& item);
  void performSetInterpretation(XCLSyntaxExpression* expr,
                       QVector<XCLProcessParameter> param,
                       FileParserState& item);
  void performAddFilter(XCLSyntaxExpression* expr,
			QVector<XCLProcessParameter> param,
                        FileParserState& item);
  void performDecompress(QByteArray* ba,
			 QVector<XCLProcessParameter> param,
                         FileParserState& state);
  _UINT8 getTypeLength(const QString& type);

};

#endif
