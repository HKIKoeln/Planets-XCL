/*
 
    * This module is part of the XCL software system
    *
    * Written by Sebastian Beyl, Elona Chudobkaite, Volker Heydegger, Jan Schnasse, Manfred Thaller 2006 - 2008
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
#include <QFile>
#include <QDataStream>
#include "XCELReader.h"

#include "XCLException.h"

#include "XCLSyntaxSymbol.h"
#include "XCLSyntaxElement.h"
#include "XCLSyntaxExpression.h"
#include "XCLSyntaxItem.h"
#include "XCLSyntaxProperty.h"
#include "XCLSyntaxProcess.h"

#include "XCLParsingSymbol.h"
#include "XCLParsingExpression.h"
#include "XCLParsingItem.h"
#include "XCLParsingProperty.h"
#include "XCLDataConverter.h"

#include "XCELProcessorInterface.h"

/*
Expressions are added to the TREE after a parseAllItem, parseSequenceItem or parseChoiceItem has been read completely and sucessfully.
Expressions are added to the INDEX as soon as they are read and returned to the parent parseAllItem, parseSequenceItem or parseChoiceItem. They are deleted if the parent does not match, i.e. an Exception is thrown.
*/

/* A few words about state.startpositions:
*  The current reading position of the parser is always available in the class variable state.startposition.
*  The previous position is always available on top of the positionStack, as long as the expression is not read
*  completely.
* 
*  The stack is only manipulated in the parse() function
*  
*  1.
*  Whenever the FileParser starts reading a new expression, the current state.startposition is pushed onto the stack.
*  If the expression doesn't match and an exception is thrown, the previous state.startposition is retrieved from the 
*  stack. state.startposition = positionStack.pop();
*  If the expression matches, the old position is popped from the stack. positionStack.pop();
*  The parser then continues reading at its new position, i.e. the state.startposition + the length of the matching 
*  expression as set in parseSymbol() or parseProperty().
 
*  2.
*  Special rules for jumps.
*  A jump occurs if an expression has a fixed startposition (this will usually be the case if a setStart() 
*  method was called by a processing) which varies from the current reading position of the file-parser.
*  In this case the current state.startposition is pushed onto the stack as usual.
*  The state.startposition is then set to the fixed value of the expression to be read. Also a jump flag is set.
*  The jump-position is pushed onto the stack as well (this will be necessary for backtracking non-matching    
*  expressions within the jump-expression)
*  When the parser finished reading the expression, the jump-position is popped from the stack as usual 
*  positionStack.pop();
*  If the jump-flag was set, the parser pops the next position from the stack and thus returns to its previous *  reading position: state.startposition = positionStack.pop();
 
Example:
*  item startpos: 0
*     symbol_1 startpos: 0, length: 5
*     symbol_2 startpos: 5, length: 6
*     processing.setStart(512) 
      item startpos: 512
	symbol startpos: 512, length: 7    
 
start reading item_1
    state.startposition = 0;
    stack{0}
    start reading symbol_1
        state.startposition = 0;    
        stack{0,0}
    end reading symbol_1
        state.startposition = 5;    
        stack{0}
    start reading symbol_2
        state.startposition = 5;    
        stack{0,5}
    end reading symbol_2
        state.startposition = 11;    
        stack{0}        
    start reading item_2
        state.startposition = 11;    
        stack{0,11,512} 
        start reading symbol
            state.startposition = 512;    
            stack{0,11,512,512} 
        end reading symbol
            state.startposition = 519;    
            stack{0,11,512} 
    end reading item_2
            stack{0,11}
            state.startposition = 11;
            stack{0}
 
 
   
*/
/*    parsing Path
*					                   parse
*                                       _____________________|_______________________
*                                      |               |           |                |
*		                   parseItem 	parseSymbol 	parseProperty 	parseProcessing
* 				/            \					/	      \
* 		parseStructuringItem 	parseSemanticItem           pushXCELProcessing    pullXCELProcessing
*             /          |         \
*  parseAllItem parseSequenceItem parseChoiceItem
*
*/
/*
*
*		    parseSymbol	<---------------_
*			|			|
*		   handleNextValue		|
*			|			|
*		 setParsedValue			|
*			|			|
*		     isValid?			|
*		  /yes	     \ no		|
*	      go ahead	    sDNMHandle->handle()  	
*
*/

/* Indices and data structures
*	
*	1. parseTree
*	2. index
*	3. grammar
*	4. grammarIndex
*
*
*	2.index
*
*	   parseSemanticItem  parseSymbol
*		 |		   |
*		 |________________|
*			|	
*		addExpressionsToIndex
*
*
*	     parseStructuring parseAll parseCHoice
*		|		|	|
*		|_______________|_______|
*		 	       |
*		    removeExpressionsFromIndex
*
*		
*/
XCELReader::XCELReader(XCELProcessorInterface* proc)
{
	processor			= proc;
	data.binaryArray	= NULL;
	data.parsedTree		= new XCLParsingItem();
	debugTree			= new XCLParsingItem();
	sDNMhandle			= new SymbolDoesNotMatchHandle();
	addressVector.append(0);
	addressPointer = 0;
}


XCELReader::~XCELReader()
{
	if(data.parsedTree != NULL)
		delete data.parsedTree;
	if(debugTree != NULL)
		delete debugTree;
	if(sDNMhandle != NULL)
		delete sDNMhandle;
}


/*! \const XCLParsingItem& XCELReader::parseFile( const QString& binFileName, const XCLSyntaxItem& gr)
 *  \brief Parses the binary array with respect to the structure gr defined by the XCELGrammarBuilder.
 *  \param binFileName The name and location of the binary file as QString.
 *  \param gr A reference to the first XCLSyntaxItem of the data.grammar.
 *  \exception XCLException Unable to read binary file.
 *  \exception WARNING Unable to create complete Tree. ParseTree will contain only matching expressions\n"
 *  \return a pointer to the root of the complete data.parseTree.
 
 * Variables to initialize:\n
 * 1. Create an Array from the binary file \n
 * 2. Initialize the data.grammar root object with \a gr and set its parent = NULL \n
 * 3. Initialize the sequentialGrammar object with the item following the data.grammar root as the first item with content to parse. \n
 * 4. Initialize the data.grammarIndex so it contains all child elements of the data.grammar root for fast access.\n
 
 * Start the recusive parsing process by calling parseItem() for the #sequentialGrammar with the #data.parsedTree as root.
 
\sa XCELGrammarBuilder
 */

const XCLParsingItem& XCELReader::parseFile(const QString& fileName,
    const XCLSyntaxItem& gr)
{
  data.binaryArray = NULL;
  //initialize variables
  try
  {
	  if(!fileName.isEmpty())
	  {
		  actSource = fileName;
		  sourceStack.push(actSource);
		  data.binaryArray = processor->switchToSource(actSource);
	  }
	  if(data.binaryArray == NULL)
	  {
		  data.binaryArray =  new QByteArray();
		  data.binaryArrayLength = LENGTH_MAX;
	  }
	  else
		  data.binaryArrayLength = data.binaryArray->size();
  }
  catch (XCLException e)
  {
	  throw XCLException(" Unable to read binary file.");
  }


  data.grammar=new XCLSyntaxItem(gr);
  data.grammar->setParent(NULL);



  XCLSyntaxItem sequentialGrammar=*((XCLSyntaxItem*)data.grammar->getFirstChild());

  data.grammarIndex = new  QHash<QString,XCLSyntaxExpression*>();
  data.grammar->addChildsToGrammarIndex(*data.grammarIndex);


  QVector<XCLParsingExpression*>* parsedExpressions = new  QVector<XCLParsingExpression*>();
  mstate.endposition = data.binaryArrayLength;
  mstate.startposition = 0;

  //start the parsing-process by calling the basic parse-method, which controls the processing of the file.
  try
  {

    parsedExpressions = parse(&sequentialGrammar, data.parsedTree, mstate);

  }
  catch(XCLOutOfRangeException exception)
  {
#ifdef PlanetsDebug
    exception.message();
#endif

  }
  catch(XCLException exception)
  {
#ifdef PlanetsDebug
    exception.message();
#endif

    throw XCLException("Root Item in XCEL doesn´t match.\n Wrong XCEL?\n");
  }

  if(data.parsedTree !=NULL)
  {
    data.parsedTree->setChildren(*parsedExpressions);
    data.parsedTree->generateIDs();
  }


#ifdef PlanetsDebug
  std::cout<<"PARSER END\n";;
#endif

  data.parsedTree->setObjectType(sequentialGrammar.getObjectType());

  delete data.grammar;
  delete data.grammarIndex;
  delete data.binaryArray;
  delete parsedExpressions;
  return *data.parsedTree;
}


/*! \QVector<XCLParsingExpression*>* XCELReader::parse(XCLSyntaxElement* element, XCLParsingItem* parent, FileParserState& state)
 *  \brief controls the recursive parsing process.
 *  \param element The data.grammar object, which describes the structure of the element the parser reads next.
 *  \param parent The parent item, i.e. the XCLParsingItem the parser has read before.
 *  \param state The parseposition and an tree-index with all elements which have been read yet.
 *  \exception XCLOutOfRangeException Element out of Range.
 *  \exception XCLException 
 *  \return A vector of matching Expressions.
 
 
if element is a processing call parseProcessing()
if element is an Expression jumps and parsing errors have to be handled too.
 
 */
QVector<XCLParsingExpression*>* XCELReader::parse(XCLSyntaxElement* element,
    XCLParsingItem* parent,
    FileParserState& state)
{
  //local Members
  QVector<XCLParsingExpression*>*  expressions = new QVector<XCLParsingExpression*>();

  //state Variables
  enum XCLSyntaxElement::ELEMENT_TYPE elementType;
  enum XCLSyntaxExpression::POS_TYPE positionType;
  enum XCLSyntaxExpression::POS_TYPE lengthType;
  BOOL jump=FALSE;
  BOOL changeInternalSource=FALSE;
  BOOL changeExternalSource=FALSE;
  //setState
  elementType=element->getElementType();

  //-------------------Processing--------------------------
  if(elementType == XCLSyntaxElement::PROCESS_TYPE)
  {

#ifdef PlanetsDebug
    std::cout<<"Parse PROCESS_TYPE->";
#endif
    positionType = XCLSyntaxExpression::UNDEFINED;
    lengthType = XCLSyntaxExpression::UNDEFINED;
    try
    {
      delete expressions;
	expressions =NULL;
      expressions=parseProcessing((XCLSyntaxProcess*)element,parent,state);

      return expressions;
    }
    catch(XCLException exception)
    {
      exception.message();
      if(expressions!=NULL)
       delete expressions;
      throw XCLException("parseProcessing failes with return FALSE\n");
    }
  }
  //-----------------------END-------------------------


  //element is an expression ...

  //update element. This is neccesary because elements can be changed by the processings
  QString id=((XCLSyntaxExpression*)element)->getIdentifier();
  XCLSyntaxExpression* expression = data.grammarIndex->value(id);


  if(expression == NULL)
    throw XCLException("XCELReader: Method parse could not find expression in data.grammarIndex\n");

  //initialize state Variables
  positionType=expression->getPositionType();

  lengthType = expression->getLengthType();

  //push current position to the stack. If the expression doesn´t match, retrieve state.startposition from stack.
  positionStack.push(state.startposition);
  // sourceStack.push(new QByteArray(*data.binaryArray));
  QString sourceString=element->getSource();
  QString extSourceString=element->getExtSource();

  if(!extSourceString.isEmpty())
  {
    jump=TRUE;
    changeExternalSource=TRUE;
    //      std::cout<<"Startposition "<<state.startposition<<"\n";
    positionStack.push(state.startposition);
    state.startposition = 0;
    sourceStack.push(actSource);
    actSource=extSourceString;
    data.binaryArray = processor->switchToSource(extSourceString);
    data.binaryArrayLength= data.binaryArray->size();
    //	std::cout<<"!!!!!!!"<<extSourceString.toStdString()<<" "<<data.binaryArrayLength<<"\n";
    //TODO: Set act Directory

  }
  else
    if(!sourceString.isEmpty())
    {
      jump=TRUE;
      changeInternalSource=TRUE;
#ifdef PlanetsDebug
      std::cout<<", Startposition("<<state.startposition<<")";
#endif
      positionStack.push(state.startposition);
      state.startposition = 0;
      internalSourceStack.push(new QByteArray(*data.binaryArray));
      delete data.binaryArray;
      *data.binaryArray = state.index.get(sourceString)->last()->getValue();
      state.index.remove(sourceString);
      data.binaryArrayLength= data.binaryArray->size();
#ifdef PlanetsDebug
      std::cout<<", ArrayLength("<<data.binaryArrayLength<<")";
#endif

    }
    else
      //if position is fixed, set current state.startposition to fixed position and mark the jump.
      if(positionType == XCLSyntaxExpression::FIXED && (state.startposition != expression->getStart()))
      {
#ifdef PlanetsDebug
        std::cout<<", info(Start is fixed)";
#endif
        jump=TRUE;
        state.startposition = expression->getStart();
        positionStack.push(state.startposition); //push jump-position onto stack

      }

  expression->setStart(state.startposition);
#ifdef PlanetsDebug
  std::cout<<", Startposition("<<state.startposition<<")";
#endif

  if ((state.startposition + expression->getLength()-1)<data.binaryArrayLength){

    expression->setEnd(state.startposition + expression->getLength()-1);
    //std::cout<<" \nstart!=end="<<expression->getIdentifier().toStdString()<<"\n";

  }

  //check range
  if(lengthType == XCLSyntaxExpression::FIXED)
  {
#ifdef PlanetsDebug
    std::cout<<", info(Length is fixed)\n";
#endif
    if(state.startposition + expression->getLength()-1 > state.endposition)
    {
      //std::cout<<"HELLO: "<< expression->getLength()<<" "<<state.endposition<<"\n";
      positionStack.pop();
      delete expressions;
      throw XCLOutOfRangeException("Element out of Range "+expression->getIdentifier()+"\n");
    }
  }

  //choose the appropriate method according to the Expression-Type (i.e. property, smbol, item or processing)
  try
  {

    if(elementType == XCLSyntaxExpression::ITEM_TYPE)
    {
#ifdef PlanetsDebug
      std::cout<<"\nParse ITEM_TYPE";
#endif
      expressions=parseItem((XCLSyntaxItem*)expression, parent, state);
    }
    else if(elementType == XCLSyntaxExpression::SYMBOL_TYPE)
    {
#ifdef PlanetsDebug
      std::cout<<"Parse SYMBOL_TYPE";
#endif
      expressions = parseSymbol((XCLSyntaxSymbol*)expression, state);
    }
    else if(elementType == XCLSyntaxExpression::PROPERTY_TYPE)
    {
#ifdef PlanetsDebug
      std::cout<<"Parse PROPERTY_TYPE";
#endif
      expressions = parseProperty((XCLSyntaxProperty*)expression, state);
    }
  }
  catch(XCLOutOfRangeException exception)
  {

#ifdef PlanetsDebug
    std::cout<<"Delete Expr "<<expression->getIdentifier().toStdString()<<"\n";
#endif
    positionStack.pop();

	if(changeInternalSource)
    {

#ifdef PlanetsDebug
      std::cout<<"sourceStack.pop() position before changeSource:"<<data.binaryArray->size()<<std::endl;
#endif

      delete data.binaryArray;
      data.binaryArray = internalSourceStack.pop();
      data.binaryArrayLength= data.binaryArray->size();
    }
    if(changeExternalSource)
    {

#ifdef PlanetsDebug
      std::cout<<"sourceStack.pop() position before changeSource:"<<data.binaryArray->size()<<std::endl;
#endif

      if(!sourceStack.isEmpty())
      {
        actSource=sourceStack.pop();
        data.binaryArray = processor->switchToSource(actSource);
        if(data.binaryArray!=NULL)
        {
          data.binaryArrayLength= data.binaryArray->size();
        }
        else
        {
          data.binaryArray =  new QByteArray();
          data.binaryArrayLength = LENGTH_MAX;
        }
      }
      else
      {
        data.binaryArray =  new QByteArray();
        data.binaryArrayLength = LENGTH_MAX;
      }
      //TODO: Set actDirectory;
    }

    delete expressions;
    throw XCLOutOfRangeException(exception.getMessage());
  }
  catch(XCLException exception)
  {
    //Item didn't match: proceed with the old parse-position
    state.startposition = positionStack.pop();
    //   sourceStack.pop();
    if(changeInternalSource)
    {

#ifdef PlanetsDebug
		std::cout<<"sourceStack.pop() position before changeSource:"<<data.binaryArray->size()<<std::endl;
#endif

      delete data.binaryArray;
      data.binaryArray = internalSourceStack.pop();
      data.binaryArrayLength= data.binaryArray->size();
    }
    if(changeExternalSource)
    {
#ifdef PlanetsDebug
      std::cout<<"sourceStack.pop() position before changeSource:"<<data.binaryArray->size()<<std::endl;
#endif

      if(!sourceStack.isEmpty())
      {
        actSource=sourceStack.pop();
        data.binaryArray = processor->switchToSource(actSource);
        if(data.binaryArray!=NULL)
        {
          data.binaryArrayLength= data.binaryArray->size();
        }
        else
        {
          data.binaryArray =  new QByteArray();
          data.binaryArrayLength = LENGTH_MAX;
        }
      }
      else
      {
        data.binaryArray =  new QByteArray();
        data.binaryArrayLength = LENGTH_MAX;
      }
      //TODO: Set actDirectory;
    }

    delete expressions;
    throw XCLException(exception.getMessage());
  }



  //Item matched: the previous state.startposition is no longer needed and therefore removed from the stack.
  positionStack.pop();

  //If jump occured, pop last state.startposition before jump
  if(jump)
  {
    state.startposition = positionStack.pop();

#ifdef PlanetsDebug
    std::cout<<"positionStack.pop() position before jump:"<<state.startposition<<std::endl;
#endif

  }
  if(changeInternalSource && !internalSourceStack.isEmpty())
  {

#ifdef PlanetsDebug
    std::cout<<"sourceStack.pop() position before changeSource:"<<data.binaryArray->size()<<std::endl;
#endif

    delete data.binaryArray;
    data.binaryArray = internalSourceStack.pop();
    data.binaryArrayLength= data.binaryArray->size();

#ifdef PlanetsDebug
    std::cout<<"sourceStack.pop() position after changeSource:"<<data.binaryArray->size()<<std::endl;
#endif

  }
  if(changeExternalSource)
  {

    if(!sourceStack.isEmpty())
    {
      actSource=sourceStack.pop();
      data.binaryArray = processor->switchToSource(actSource);
      if(data.binaryArray!=NULL)
      {
        data.binaryArrayLength= data.binaryArray->size();
      }
      else
      {
        data.binaryArray =  new QByteArray();
        data.binaryArrayLength = LENGTH_MAX;
      }
    }
    else
    {
      data.binaryArray =  new QByteArray();
      data.binaryArrayLength = LENGTH_MAX;
    }
    //TODO: Set actDirectory;
  }

  return expressions;
}


/*! \QVector<XCLParsingExpression*>* XCELReader::parseItem(XCLSyntaxItem* syntaxItem, XCLParsingItem* parent, FileParserState& state)
 *  \brief Handles parsing ranges for items and delegates the parsing process to the appropriate function.
 *  \param syntaxItem The data.grammar object, which describes the structure of the item the parser reads next.
 *  \param parent The parent item, i.e. the XCLParsingItem the parser has read before.
 *  \param state The parseposition and an tree-index with all elements which have been read yet.
 *  \exception XCLOutOfRangeException Item range is set to old Range.
 *  \exception XCLException 
 *  \return A vector of matching Expressions.
 
 
*		                   parseItem 	
* 				/            \	
* 		parseStructuringItem 	parseSemanticItem  
*             /          |         \
*  parseAllItem  parseSequenceItem  parseChoiceItem
 
 
Ranges:
during parsing of an item the FileParserState endposition holds the local parsing range of the item to read. 
After the item has been read the endposition is reset to the value it contained before, i.e. the range of its parent item. 
 
In case the parsing process fails the matchingExpressions vector is deleted and an Exception is thrown.
Else a vector containing the matching Expressions is returned to the central function parse().
 
 */
QVector<XCLParsingExpression*>* XCELReader::parseItem(	XCLSyntaxItem* syntaxItem,
    XCLParsingItem* parent,
    FileParserState& state)
{



  QVector<XCLParsingExpression*>*  matchingExpressions = new QVector<XCLParsingExpression*>();
  _LENGTH oldrange = state.endposition;
  QVector<XCLParsingExpression*>*  newExpressions;
  try
  {
    if (syntaxItem->getLength() != LENGTH_MAX )
    {
      //syntaxItem->print();
      state.endposition = syntaxItem->getEnd();
    }

    if ( syntaxItem->getItemType() == XCLSyntaxItem::STRUCTURING )
    {
      newExpressions = parseStructuringItem(syntaxItem, parent, state);
      *matchingExpressions+=*(newExpressions);
      delete newExpressions;

    }
    else if (syntaxItem->getItemType() == XCLSyntaxItem::CONTEXT
             ||syntaxItem->getItemType() == XCLSyntaxItem::DEFINITION)
    {
      newExpressions = parseSemanticItem(syntaxItem, state);
      *matchingExpressions+=*(newExpressions);
      delete newExpressions;
    }
  }
  catch(XCLOutOfRangeException exception)
  {
#ifdef PlanetsDebug
    std::cout<<"XCLOutOfRangeException caught in parseItem\n";
#endif
    state.endposition = oldrange;
    delete matchingExpressions;
    throw XCLOutOfRangeException("Item range is set to old Range "+syntaxItem->getIdentifier()+"\n");
  }
  catch(XCLException exception)
  {
#ifdef PlanetsDebug
    std::cout<<"XCLException caught in parseItem "<<syntaxItem->getIdentifier().toStdString()<<"\n";
#endif
    state.endposition = oldrange;
    delete matchingExpressions;
    throw XCLException(exception.getMessage());
  }

  state.endposition = oldrange;

  if(syntaxItem->getPrintMe())
  {
    std::cout<<"Print: ";
    for(int i=0;i< matchingExpressions->size();i++)
      std::cout<<"MatchingItem: "<<matchingExpressions->at(i)->getIdentifier().toStdString()<<" "<<matchingExpressions->at(i)->getInterpretedValue().toStdString()<<"\n";//->print();



    QList<XCLParsingExpression*>* list = mstate.index.get(matchingExpressions->at(0)->getIdentifier());
    for(int i=0;i<list->size();i++)
    {
      std::cout<<list->at(i)->getInterpretedValue().toStdString()<<"\n";
    }
  }



  return matchingExpressions;
}


/*! \QVector<XCLParsingExpression*>* XCELReader::parseStructuringItem(XCLSyntaxItem* syntaxItem, XCLParsingItem* parent, FileParserState& state)
 *  \brief Calls appropriate method according to the items content model (i.e. all, sequence, choice).
 *  \param syntaxItem The data.grammar object, which describes the structure of the item the parser reads next.
 *  \param parent The parent item, i.e. the XCLParsingItem the parser has read before.
 *  \param state The parseposition and an tree-index with all elements which have been read yet.
 *  \exception XCLOutOfRangeException 
 *  \exception XCLException Item has invalid order-type!
 *  \exception XCLException 
 *  \return A vector of matching Expressions.
 
	
* 		parseStructuringItem 	 
*             /          |         \
*  parseAllItem  parseSequenceItem  parseChoiceItem
 
* In case the parsing process fails the matchingExpressions vector is deleted and an Exception is thrown.
* Else a vector containing the matching Expressions is returned to parseItem().
*/

QVector<XCLParsingExpression*>* XCELReader::parseStructuringItem(XCLSyntaxItem* syntaxItem,
    XCLParsingItem* parent,
    FileParserState& state)
{

  QVector<XCLParsingExpression*>*  matchingExpressions = new QVector<XCLParsingExpression*>();
  //call appropriate method according to the items content model (i.e. all, sequence, choice)
  enum XCLSyntaxItem::ORDER_TYPE orderType = ((XCLSyntaxItem*)syntaxItem)->getOrderType();
  try
  {
    switch(orderType)
    {
    case XCLSyntaxItem::ALL:
      matchingExpressions=parseAllItem(syntaxItem, parent, state);
      break;
    case XCLSyntaxItem::CHOICE:
      matchingExpressions=parseChoiceItem(syntaxItem, parent, state);
      break;
    case XCLSyntaxItem::SEQUENCE:
      matchingExpressions=parseSequenceItem(syntaxItem, parent, state);
      break;
    default:
      throw XCLException("Item has invalid order-type! "+syntaxItem->getIdentifier()+"\n");
    };
  }
  catch(XCLOutOfRangeException exception)
  {
    delete matchingExpressions;
    throw XCLOutOfRangeException(exception.getMessage());
  }
  catch(XCLException exception)
  {
    delete matchingExpressions;
#ifdef PlanetsDebug
    std::cout<<"XCLException caught in parseStructuringItem "<<syntaxItem->getIdentifier().toStdString()<<"\n";
#endif
    throw XCLException(exception.getMessage());
  }

  return matchingExpressions;
}


/*! \QVector<XCLParsingExpression*>* XCELReader::parseSemanticItem(XCLSyntaxItem* syntaxItem, XCLParsingItem* parent, FileParserState& state)
 *  \brief .
 *  \param syntaxItem The data.grammar object, which describes the structure of the item the parser reads next.
 *  \param state The parseposition and an tree-index with all elements which have been read yet.
 *  \exception XCLException 
 *  \return A vector of matching Expressions.
 
	
*/
QVector<XCLParsingExpression*>* XCELReader::parseSemanticItem(
  XCLSyntaxItem* 	element,
  FileParserState& state)
{
#ifdef PlanetsDebug
  std::cout<<", SEMANTIC ITEM";
#endif

  QVector<XCLParsingExpression*>*  matchingExpressions = new QVector<XCLParsingExpression*>();
  QVector<XCLParsingExpression*>*  expressions = new QVector<XCLParsingExpression*>();

  XCLParsingItem* parsingItem = pFactory->create(*element);
  parsingItem->setStart( state.startposition );
  XCLSyntaxItem oldItem = *element; //remember the original item

  //fake the behaviour, don´t forget: now the data.grammarIndex is wrong
  element->setItemType(XCLSyntaxItem::STRUCTURING);

  //element->setOrderType(XCLSyntaxItem::SEQUENCE);
  state.isInDefinitionContext = TRUE;
  try
  {
    expressions = parse(element,parsingItem,state);

    if (!expressions->isEmpty())
    {

      parsingItem->setChildren(*expressions);

      //set back the instance the data.grammarIndex is pointing too.
      *element=oldItem;


      // parent->add(parsingItem);
#ifdef PlanetsDebug
      std::cout<<", Semantic AddExpressions(";
#endif
       if(state.isInDefinitionContext
		||
	!parsingItem->getIdentifier().startsWith("extrc_") 
		|| 
	!parsingItem->getName().isEmpty())
      {
   //     std::cout<<"expressions AdD: "<< parsingItem->getIdentifier().toStdString()<<"\n";

        matchingExpressions->append(parsingItem);
      }
    }
  }
  catch(XCLException exception)
  {
#ifdef PlanetsDebug
    std::cout<<"XCLException caught in parseSemanticItem "<<element->getIdentifier().toStdString()<<"\n";
#endif
    //set back the instance the data.grammarIndex is pointing too.
    *element=oldItem;
    delete matchingExpressions;
    state.isInDefinitionContext=FALSE;
    throw XCLException(exception.getMessage());
  }
  delete expressions;
  state.addExpressionsToIndex( matchingExpressions);
  state.isInDefinitionContext=FALSE;
  return matchingExpressions;
}





QVector<XCLParsingExpression*>* XCELReader::parseAllItem(XCLSyntaxItem* syntaxItem, 		XCLParsingItem* parent, FileParserState& state)
{
#ifdef PlanetsDebug
  std::cout<<"All Item\n";
#endif

  //all matching Expressions
  QVector <XCLParsingExpression*>* matchingExpressions = new QVector <XCLParsingExpression*>();


  BOOL result=FALSE; // is true if the byteSequence described by syntaxItem matches to its description
  BOOL oneHasMatched=FALSE; // is TRUE if a new child matches at the current position

  QVector<XCLSyntaxElement*> expressions;
  expressions = syntaxItem->getChildren();
  QVector<XCLParsingExpression*>* newExpressions;
  do
  {
    oneHasMatched=FALSE;
    _LENGTH size=expressions.size();
    for(_LENGTH i=0;i<size;i++)
    {

      try
      {
        if(expressions.at(i)->getElementType() == XCLSyntaxElement::PROCESS_TYPE)
        {
          newExpressions = parse(expressions.at(i), parent,state);
          delete matchingExpressions;
          *matchingExpressions=*(newExpressions);
          delete newExpressions;
          continue;
        }

        XCLSyntaxExpression* expr = (XCLSyntaxExpression*)expressions.at(i);
        BOOL isMultiple = expr->getIsMultiple();
        BOOL isOptional = expr->getIsOptional();

        //----------------!isOptional && isMultiple -----------------------//
        if(!isOptional && isMultiple)
        {
          oneHasMatched=parseMultiple(expr, parent, state, *matchingExpressions);
          if(oneHasMatched)
            expr->setIsOptional(TRUE);
        
        }
        //----------------!isOptional && !isMultiple -----------------------//
        else if (!isOptional && !isMultiple)//is not Multiple
        {
          oneHasMatched=parseSimple(expr, parent, state, *matchingExpressions);
          if(oneHasMatched)
            expressions.remove(i);
        }
        //----------------isOptional && isMultiple -----------------------//
        else if(isOptional && isMultiple)
        {
          oneHasMatched=parseMultiple(expr, parent, state, *matchingExpressions);
        }
        //----------------isOptional && !isMultiple -----------------------//
        else if(isOptional && !isMultiple)
        {
          oneHasMatched=parseSimple(expr, parent, state, *matchingExpressions);
          if(oneHasMatched)
            expressions.remove(i);
        }
        if (!result)
          result = oneHasMatched;

      }
      catch(XCLOutOfRangeException exception)
      {
#ifdef PlanetsDebug
        std::cout<<"All Item catch XCLOutOfRangeException"<<i<<"\n";
#endif
      }
      catch(XCLException exception)
      {
#ifdef PlanetsDebug
        std::cout<<"removeExpressionsFromIndex\n";
#endif
        state.removeExpressionsFromIndex(matchingExpressions);
#ifdef PlanetsDebug
        std::cout<<"All Item catch "<<i<<"\n";
#endif
#ifdef PlanetsDebug
        exception.message();
#endif

      }
      if(oneHasMatched)
      {
#ifdef PlanetsDebug
        std::cout<<"addExpressionsToIndex\n";
#endif
       
        break; //up to the while
      }
    }

  }
  while(oneHasMatched);


  _LENGTH size = expressions.size();
  for(_LENGTH i=0;i<size;i++)
  {
    if(expressions.at(i) !=NULL)
      if(expressions.at(i)->getElementType()!=XCLSyntaxElement::PROCESS_TYPE)
        if(!((XCLSyntaxExpression*)expressions.at(i))->getIsOptional())
        {
          state.removeExpressionsFromIndex( matchingExpressions);
          delete matchingExpressions;
          throw XCLException("All Item did not match. not all nonoptional children have matched  "+syntaxItem->getIdentifier()+"\n");
        }
  }



  parent->setEnd( state.startposition );
  return matchingExpressions;
}


QVector<XCLParsingExpression*>* XCELReader::parseSequenceItem(XCLSyntaxItem* syntaxItem,
    XCLParsingItem* parent,
    FileParserState& state)
{

  BOOL error=FALSE; //ACHTUNG nur eine OutOfRangeException wird gefangen!!


  QVector <XCLParsingExpression*>* matchingExpressions = new QVector<XCLParsingExpression*>();

  BOOL result=FALSE;
  BOOL oneHasMatched=FALSE;

  QVector<XCLSyntaxElement*> syntaxExpressions;
  syntaxExpressions = syntaxItem->getChildren();

  _LENGTH size = syntaxExpressions.size();
  for(_LENGTH i=0;i<size;i++)
  {
    oneHasMatched=FALSE;


    if(syntaxExpressions.at(i)->getElementType() == XCLSyntaxElement::PROCESS_TYPE)
    {

      QVector <XCLParsingExpression*>* mEs=parse(syntaxExpressions.at(i), parent,state);

      if(mEs !=NULL)
        *matchingExpressions+=*(mEs);
   

      continue;
    }

    XCLSyntaxExpression* expr = (XCLSyntaxExpression*)syntaxExpressions.at(i);


    BOOL isMultiple = expr->getIsMultiple();
    BOOL isOptional = expr->getIsOptional();

    try
    {
      //----------------isOptional && isMultiple -----------------------//
      if(!isOptional && isMultiple)
      {
        result=parseMultiple(expr, parent, state, *matchingExpressions);
      }

      //----------------!isOptional && !isMultiple -----------------------//
      else if (!isOptional && !isMultiple)//is not Multiple
      {
        result=parseSimple(expr, parent, state, *matchingExpressions);
      }

      //----------------isOptional && isMultiple -----------------------//
      else if(isOptional && isMultiple)
      {
        oneHasMatched=parseMultiple(expr, parent, state, *matchingExpressions);

        if(oneHasMatched==TRUE)
        {

          result =TRUE;
        }
      }

      //----------------isOptional && !isMultiple -----------------------//
      else if(isOptional && !isMultiple)
      {
        oneHasMatched=parseSimple(expr, parent, state, *matchingExpressions);
        if(oneHasMatched==TRUE)
        {
          result =TRUE;
        }

      }

    }
    catch(XCLOutOfRangeException exception)
    {

      if(error)
      {
        delete matchingExpressions;
        throw XCLOutOfRangeException(exception.getMessage());
      }

      error=TRUE;//!!!!!!!!ACHTUNG
    }

    if(!result)
    {
#ifdef PlanetsDebug
      std::cout<<"removeExpressionsFromIndex\n";
#endif
      state.removeExpressionsFromIndex( matchingExpressions);
      delete matchingExpressions;

      throw XCLException("Sequence Item did not match "+syntaxItem->getIdentifier()+"\n");
    }
  
  }

  
  parent->setEnd( state.startposition );


#ifdef PlanetsDebug
  std::cout<<"removeExpressionsFromIndex\n";
#endif
  return matchingExpressions;
}








//QCHOICE
QVector<XCLParsingExpression*>* XCELReader::parseChoiceItem(XCLSyntaxItem* syntaxItem,
    XCLParsingItem* parent,
    FileParserState& state)
{
  QVector <XCLParsingExpression*>* matchingExpressions = new QVector <XCLParsingExpression*>();

  BOOL result=FALSE; // is true if the byteSequence described by syntaxItem matches to its description
  BOOL oneHasMatched=FALSE; // is TRUE if a new child matches at the current position

  QVector<XCLSyntaxElement*> syntaxExpressions;
  syntaxExpressions = syntaxItem->getChildren();
  QVector<XCLParsingExpression*>* newExpressions;
  try
  {
    oneHasMatched=FALSE;
    _LENGTH size=syntaxExpressions.size();
    for(_LENGTH i=0;i<size;i++)
    {
      if(syntaxExpressions.at(i)->getElementType() == XCLSyntaxElement::PROCESS_TYPE)
      {
        newExpressions=parse(syntaxExpressions.at(i), parent,state);
        delete matchingExpressions;
        *matchingExpressions=*(newExpressions);
        delete newExpressions;
        continue;
      }
      XCLSyntaxExpression* expr = (XCLSyntaxExpression*)syntaxExpressions.at(i);
      BOOL isMultiple = expr->getIsMultiple();
      if(isMultiple)
      {
        oneHasMatched=parseMultiple(expr, parent, state, *matchingExpressions);
        if(oneHasMatched==TRUE)
        {
          result=TRUE;
        }
      }
      else if(!isMultiple)
      {
        result=parseSimple(expr, parent, state, *matchingExpressions);
      }
      if(result) break;
    }
  }
  catch(XCLOutOfRangeException exception)
  {

  }
  if(!result)
  {
    state.removeExpressionsFromIndex(matchingExpressions);
    delete matchingExpressions;
    throw XCLException("Choice Item did not match. none of my children has matched  "+syntaxItem->getIdentifier()+"\n");
  }
  else
  {
    parent->setEnd( state.startposition );
  }
  return matchingExpressions;
}












/*! \BOOL XCELReader::parseMultiple(XCLSyntaxElement* element,
                                 XCLParsingItem* parent,
                                 FileParserState& state,
                                 QVector <XCLParsingExpression*>& matchingExpressions)
 *  \brief Reads an element with multiple set to True.
 *  \param element The data.grammar object, which describes the structure of the element the parser reads next.
 *  \param parent The parent item, i.e. the XCLParsingItem the parser has read before.
 *  \param state The parseposition and an tree-index with all elements which have been read yet.
 *  \param matchingExpressions A vector of matching Expressions.
 *  \exception XCLOutOfRangeException 
 *  \exception XCLException 
 
 *  \return TRUE if the element has matched at least one time otherwise returns false.
 
 * Calls parse() and adds the result to the vector of matchingExpressions or throws an Exception in case of failure.
 
 */
BOOL XCELReader::parseMultiple(XCLSyntaxExpression* element,
                               XCLParsingItem* parent,
                               FileParserState& state,
                               QVector <XCLParsingExpression*>& matchingExpressions)
{
  //std::cout<<"Enter Multiple\n";
  state.sDMhandleIsActive=TRUE;
  BOOL oneHasMatched=FALSE;
  QVector<XCLParsingExpression*>* newExpressions = NULL;
  do
  {
    try
    {
      newExpressions = parse(element, parent, state);
      matchingExpressions+=(*newExpressions);
      delete newExpressions;
      oneHasMatched=TRUE;

      state.sDMhandleIsActive=FALSE;
      if (element->getIdentifier()=="dummy" || element->getName()=="dummy")
        break;
    }
    catch(XCLOutOfRangeException exception)
    {
#ifdef PlanetsDebug
      exception.message();
#endif

      if(state.startposition >= data.binaryArrayLength && !oneHasMatched)
      {
        if(newExpressions != NULL)
          delete newExpressions;
        throw XCLOutOfRangeException(exception.getMessage());
      }
      else
        break;
    }
    catch(XCLException exception)
    {
#ifdef PlanetsDebug
      exception.message();
#endif
      break;
    }
    element->setStart(state.startposition);
  }
  while(oneHasMatched);

  return oneHasMatched;
}


/*! \BOOL XCELReader::parseSimple(XCLSyntaxElement* element,
                                 XCLParsingItem* parent,
                                 FileParserState& state,
                                 QVector <XCLParsingExpression*>& matchingExpressions)
 *  \brief Reads an element with multiple set to False.
 *  \param element The data.grammar object, which describes the structure of the element the parser reads next.
 *  \param parent The parent item, i.e. the XCLParsingItem the parser has read before.
 *  \param state The parseposition and an tree-index with all elements which have been read yet.
 *  \param matchingExpressions A vector of matching Expressions.
 *  \exception XCLOutOfRangeException 
 *  \exception XCLException 
 
 *  \return TRUE if the element has matched otherwise returns false.
 
 * Calls parse() and adds the result to the vector of matchingExpressions or throws an Exception in case of failure.
 
 */
BOOL XCELReader::parseSimple(XCLSyntaxElement* element,
                             XCLParsingItem* parent,
                             FileParserState& state,
                             QVector <XCLParsingExpression*>& matchingExpressions)
{
  QVector<XCLParsingExpression*>* newExpressions = NULL;
  state.sDMhandleIsActive=TRUE;
  BOOL oneHasMatched=FALSE;
  try
  {
    newExpressions = parse((XCLSyntaxExpression*)element, parent, state);
    matchingExpressions+=(*newExpressions);
    delete newExpressions;
    oneHasMatched=TRUE;
  }
  catch(XCLOutOfRangeException exception)
  {
    if(newExpressions != NULL)
      delete newExpressions;
    throw XCLOutOfRangeException(exception.getMessage());
  }
  catch(XCLException exception)
  {
#ifdef PlanetsDebug
    exception.message();
#endif

  }
  return oneHasMatched;
}


/*! \QVector<XCLParsingExpression*>* XCELReader::parseProperty(XCLSyntaxProperty* syntaxProperty, FileParserState& state)
 *  \brief Creates a XCLParsingProperty from the data.grammar object and reads its value from the bytestream.
 *  \param syntaxSymbol The data.grammar object, which describes the structure of the element the parser reads next.
 *  \param state The parseposition and an tree-index with all elements which have been read yet.
 *  \exception XCLException Property doesn´t match.
 *  \exception XCLException Property is not valid.
 *  \exception XCLOutOfRangeException Symbol out of Range.
 *  \return The matching Property in an XCLParsingExpression vector or an empty vector.
 
 * sets the properties value by calling setParsingValue() 
        throws an Exception if the properties doesn't match the bytestream. 
 
 * checks the properties validity by calling isValid()
        throws an Exception if the property is not valid
 
 * if the property is valid the Fileparser continues reading after the properties endposition.
 
 */
QVector<XCLParsingExpression*>* XCELReader::parseProperty(XCLSyntaxProperty* syntaxProperty, FileParserState& state)
{

  XCLParsingProperty* property = pFactory->create(*syntaxProperty);
  return handleNextValue(property,syntaxProperty,state);

}


/*! \QVector<XCLParsingExpression*>* XCELReader::parseSymbol(XCLSyntaxSymbol* syntaxSymbol, FileParserState& state)
 *  \brief Creates a XCLParsingSymbol from the data.grammar object and reads its value from the bytestream.
 *  \param syntaxSymbol The data.grammar object, which describes the structure of the element the parser reads next.
 *  \param state The parseposition and an tree-index with all elements which have been read yet.
 *  \exception XCLException Symbol doesn´t match.
 *  \exception XCLException Symbol is not valid.
 *  \exception XCLOutOfRangeException Symbol out of Range.
 *  \return The matching Symbol in an XCLParsingExpression vector or an empty vector.
 
 * sets the symbols value by calling setParsingValue() 
        throws an Exception if the symbol doesn't match the bytestream. 
 
 * checks the sybols validity by calling isValid()
        throws an Exception if the symbol is not valid
 
 * if the symbol is valid the Fileparser continues reading after the symbols endposition.
 
 */
QVector<XCLParsingExpression*>* XCELReader::parseSymbol(XCLSyntaxSymbol* syntaxSymbol, FileParserState& state)
{
#ifdef PlanetsDebug
  std::cout<<"Reader: ParseSymbol\n";
#endif
  XCLParsingSymbol* symbol = pFactory->create(*syntaxSymbol);

  return handleNextValue(symbol,syntaxSymbol,state);
}


QVector<XCLParsingExpression*>* XCELReader::handleNextValue(XCLParsingSymbol* pExpr,
    XCLSyntaxSymbol* syntaxSymbol,
    FileParserState& state)
{

  QVector<XCLParsingExpression*>*  expressions = new QVector<XCLParsingExpression*>();

  while(TRUE)
  {
    if (!setParsingValue(pExpr))
    {
      delete expressions;
      throw XCLException("Symbol did not match "+syntaxSymbol->getIdentifier()+"\n");
    }
    if(!pExpr->isValid())
    {
      if(!sDNMhandle->handle(pExpr,data.binaryArray) || !state.sDMhandleIsActive)
      {
        delete expressions;
        throw XCLException("Symbol is not valid "+syntaxSymbol->getIdentifier()+"\n");
      }

    }
    else
    {

      state.startposition=pExpr->getEnd()+1;

      if(state.isInDefinitionContext || !pExpr->getIdentifier().startsWith("extrc_") || !pExpr->getName().isEmpty())
      {
 //       std::cout<<"expressions AdD: "<< pExpr->getIdentifier().toStdString()<<"\n";
        expressions->append(pExpr);
      }

#ifdef PlanetsDebug
      debugTree->add(  pExpr);
#endif

	  state.addExpressionsToIndex( expressions);
      return expressions;
    }

  }
  delete expressions;
  return NULL;
}


/*! \BOOL XCELReader::setParsingValue(XCLParsingSymbol* symbol)
 *  \brief Reads the value of a symbol from the binary file and saves the uncompressed raw data as well as the interpreted data.
 *  \param symbol The XCLParsingSymbol which describes how to interpret the value.
 *  \return FALSE if the reading position for the value extends the size of the binary file otherwise returns TRUE
 
A symbols value is represented in two ways: 
1. the uncopressed raw data as a QByteArray #value.
2. the data interpreted with respect to its endianess, data type or labelling as a QString #interpretedValue.
 
This function reads "length" bytes found at the fileposition "start" from the data.binaryArray-> If the symbol has a filterChain the filter is applied to the data and the converted data is saved as the symbols raw value to the QByteArray.
It then calls the XCLInputNormalizer and saves the result as the symbols interpretedValue.
 
 */
BOOL XCELReader::setParsingValue(XCLParsingSymbol* symbol)
{

  _LENGTH start=symbol->getStart();
  _LENGTH length;

  if(props.addrScheme == ReaderProperties::SEPARATOR && symbol->getLength(data.binaryArray)!=0)
  {
      _SINT64 tmp = data.binaryArray->indexOf(props.addressSeperator, start);
      // ### An dieser Stelle habe ich gecastet, um die Vorzeichen aus den
      // Compiler-warnings zu holen. Meiner Meinung nach muesste das moeglich sein,
      // aber ich muss das noch mit Elona abklaeren.
      if(tmp == _SINT64(start))
        tmp = data.binaryArray->indexOf(props.addressSeperator, start+1);
      // Falls tmp==-1, kommt SEPARATOR in binaryArray nicht vor.
      // In diesem Fall soll die letzte Position des binaryArray's zurückgeliefert werden.
      // Falls binaryArray keinen Separator enthält, wird der ganze binaryArray zurückgegeben.
      // Dies wird benuzt bei ImageMagick.
      if(tmp==-1 && start!=data.binaryArrayLength)
	tmp=data.binaryArrayLength;
      length = tmp - start;
      symbol->setLength(length);
  }
  else
  {

    length=symbol->getLength(data.binaryArray);
  }

  QByteArray parsedValue; //raw value
  QString* interpretedValue = NULL; //interpreted Value

  if(start > data.binaryArrayLength)
    return FALSE;

  if(start+length<=data.binaryArrayLength)
  {

    parsedValue = data.binaryArray->mid((int)start,(int)length);

  }
  else
  {

    throw XCLOutOfRangeException("Exception in XCLReader::setValue(): calculated length is out of range");

  }

  symbol->setValue(parsedValue);

  
  //if symbol has filterChain convert value and set converted value
  if ((symbol->getFilterChain()).size() >= 1)
  {
    XCLDataConverter convert;
    parsedValue = convert.filter(&parsedValue, symbol->getFilterChain());
    symbol->setValue(parsedValue);
  }

  if( symbol->getEncoding() != "byte" )
  {
    interpretedValue = new QString();
    encoder.setEncoding(symbol->getEncoding());
    length = encoder.get(*interpretedValue, data.binaryArray, start, length);

    symbol->setLength(length);

    symbol->setInterpretedValue(*interpretedValue);
 
    delete interpretedValue;
     interpretedValue = NULL;
  }
  else
    if(symbol->getInterpretationType()!=XCLParsingExpression::NUMERIC && parsedValue.size()!=0)
    {

      //interpret value as label or according to its numeric or character respresentation
      XCLInputNormalizer normalizer;
      interpretedValue=normalizer.normalizeValue((UCHAR*)parsedValue.data(),
                       parsedValue.size(),
                       symbol->getInterpretation(),
                       symbol->getIsBigEndian());



      symbol->setInterpretedValue(*interpretedValue);
      delete interpretedValue;
      interpretedValue = NULL;
    }
  if(symbol->getPrintMe())
    symbol->print();

#ifdef PlanetsDebug
  std::cout<<"\n ------------------------------------------------\n|MATCHING SYMBOL\t"<<symbol->getIdentifier().toStdString()<<"\t|";
#endif

  /* Sebastian Beyl, 23.2.2009: Zu einem frueheren Zeitpunkt war die Zeile symbol->print() innerhalb des
     PlanetsDebug-Bereichs. Allerdings ist das Programm als Release-Version dann immer abgestuerzt. Die Ursachen
	 sind mir noch nicht so ganz klar. */
  symbol->print();

  if(interpretedValue!=NULL)
    delete interpretedValue;
  return TRUE;
}


/*! \void XCELReader::createBinaryArray(QString binFileName)
 *  \brief Reads the binary file into a QByteArray hold by the class.
 *  \param binFileName The name and location of the binary file as QString.
 *  \exception XCLException Couldn´t open file.
 
 * initializes the class member variable _LENGTH #data.binaryArrayLength with the size of the QByteArray.
 */
void XCELReader::createBinaryArray(QString binFileName)
{

  QFile f( binFileName );
  if ( !f.open(QFile::ReadOnly) )
    throw XCLException("Couldn´t open file:"+binFileName);
  data.binaryArray->resize( (uint)f.size() );
  f.read( data.binaryArray->data(), (uint)f.size() );	// read file into byte array
  f.close();

  data.binaryArrayLength=data.binaryArray->size();
}


void XCELReader::setParseTreeFactory(XCLParseTreeFactory* factory)
{
  pFactory=factory;
}


void XCELReader::setProcessor(XCELProcessorInterface* proc)
{
  processor=proc;
}


QVector<XCLParsingExpression*>* XCELReader::parseProcessing(
  XCLSyntaxProcess* element,
  XCLParsingItem* parent,
  FileParserState& state)
{

  state.parent=parent;
  state.actElement=element;
  QVector<XCLParsingExpression*>*  expressions = processor->process();

  return expressions;
}


void XCELReader::print(XCLSyntaxElement* element)
{
  print();
  element->print();
}


void XCELReader::print(XCLParsingExpression* element)
{
  print();
  element->print();
}


void XCELReader::print()
{
  std::cout<<"Print FilePositions: \n";
  QVector<_LENGTH> positions = positionStack;
  for(int i=0;i<positions.size();i++)
  {
    std::cout<<"|"<<positions[i]<<" ";
  }
  std::cout<<"\n";
}



BOOL SymbolDoesNotMatchHandle::handle(XCLParsingExpression* expr, QByteArray* ba)
{
  return FALSE;
  // Useless casting for less compiler-warnings
  (void)expr;
  (void)ba;
}


BOOL SkipWhiteSpaceHandle::handle(XCLParsingExpression* expr, QByteArray* ba)
{

  BOOL hasSkiped=FALSE;
  int offset= expr->getStart();
  UCHAR num;

  if(offset<ba->size()-1 && offset >= 0)
  {
    num=ba->at(offset++);
  }
  else
    return FALSE;
  int i=0;
  while(num == 0 || num == 9 || num == 10 || num == 12 || num == 13|| num == 32)
  {
  

    if(offset<ba->size()-1)
    {
      num=ba->at(offset++);
      i++;
      hasSkiped=TRUE;
    }
    else
	break;	

  }

  expr->setStart(offset-1);

  return hasSkiped;
}

const XCLParsingItem& XCELReader::getDebugTree()
{
  return *debugTree;
}
FileParserState* XCELReader::getState()
{
  return &mstate;
}
QByteArray* XCELReader::getBinaryArray()
{
  return data.binaryArray;
}
void XCELReader::setSymbolDoesNotMatchHandle(SymbolDoesNotMatchHandle* h)
{
  sDNMhandle =h;
}
void XCELReader::setAddressingScheme(const QString& as, const QByteArray& addrSep)
{
  if(as == "separator")
    props.addrScheme = ReaderProperties::SEPARATOR;
  else if(as == "byte")
    props.addrScheme = ReaderProperties::BYTE;

  props.addressSeperator = addrSep;
  ;
}
void XCELReader::gotToLastAdress( )
{
  if(props.addrScheme == ReaderProperties::SEPARATOR)
  {
    int tmp;
    while(-1!=(tmp=goToNextAddress()))
    {
      ;
    }
    addressVector.append(data.binaryArrayLength-1);
    addressPointer=addressVector.size()-1;
  }
  else if(props.addrScheme == ReaderProperties::BYTE)
  {
    mstate.startposition = data.binaryArrayLength-1;
  }
  goToPreviousAddress();
  //std::cout<<"Go To Last Address: "<<mstate.startposition<<"\n";
}
void XCELReader::goToPreviousAddress( )
{
  if(props.addrScheme == ReaderProperties::SEPARATOR)
  {
    addressPointer--;
    mstate.startposition = addressVector.at(addressPointer);

  }
  else if(props.addrScheme == ReaderProperties::BYTE)
  {
    mstate.startposition--;
  }

}


int XCELReader::goToNextAddress( )
{
  if(props.addrScheme == ReaderProperties::SEPARATOR)
  {

    int tmp=data.binaryArray->indexOf(props.addressSeperator,mstate.startposition);

    if(tmp == mstate.startposition)
    {
      if(tmp != data.binaryArrayLength)
      {
        tmp = data.binaryArray->indexOf(props.addressSeperator, mstate.startposition + 1);
      }
      else
      {
        tmp = -1;
      }
    }
    if(tmp != -1)
    {

      if(tmp != data.binaryArrayLength - 1)
      {
        mstate.startposition = tmp;

        addressVector.append(mstate.startposition);
        addressPointer = addressVector.size() - 1;
      }
      else
      {
        tmp = -1;
      }
    }
    return tmp;
  }
  else if(props.addrScheme == ReaderProperties::BYTE)
  {
    mstate.startposition++;
  }
  return mstate.startposition;
}
XCLSyntaxItem* XCELReader::getGrammar()
{
  return data.grammar;
}

QHash<QString,XCLSyntaxExpression*>* XCELReader::getGrammarIndex()
{
  return data.grammarIndex;
}
void XCELReader::registerParsedValues(QVector<XCLParsingExpression*>* expressions)
{
  mstate.addExpressionsToIndex(expressions);
}
void XCELReader::registerParsedValues(XCLTree<XCLParsingExpression*>* i)
{
  mstate.index.insert(i);
}


void FileParserState::addExpressionsToIndex(QVector<XCLParsingExpression*>* expressions)
{

  _LENGTH size = expressions->size();
  for(_LENGTH i=0;i<size;i++)
  {
    if(!expressions->at(i)->getIdentifier().startsWith("extrc_"))
    {
      //	std::cout<<"ADD: "<<expressions->at(i)->getIdentifier().toStdString()<<"\n";
      index.insert(expressions->at(i)->getIdentifier(),expressions->at(i));
    }

  }
}
void FileParserState::addExpressionToIndex(XCLParsingExpression* expression)
{
  if(!expression->getIdentifier().startsWith("extrc_"))
  {
    // std::cout<<"ADD: "<<expression->getIdentifier().toStdString()<<"\n";
    index.insert(expression->getIdentifier(),expression);
  }
}

void FileParserState::removeExpressionsFromIndex( QVector <XCLParsingExpression*>* expressions)
{
  // ### Wird size tatsaechlich nicht verwendet? Und wenn, muesste es eigentlich ein int sein _SINT32
  _LENGTH size = expressions->size();
  (void)size;

  for(_SINT32 i=0; i < expressions->size(); i++)
  {
    if(!expressions->at(i)->getIdentifier().startsWith("extrc_"))
    {
      // std::cout<<"REMOVE: "<<expressions->at(i)->getIdentifier().toStdString()<<"\n";
      index.remove(expressions->at(i)->getIdentifier());

    }
  }
}
