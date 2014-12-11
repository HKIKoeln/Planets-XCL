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

#ifndef XCELREADER_H
#define XCELREADER_H
#include "XCLParsingItem.h"
#include "XCLSyntaxItem.h"
#include "Defines.h"
#include <QHash>
#include <QStack>
#include "XCLInputNormalizer.h"
#include "XCLTree.h"
#include "XCLParseTreeFactory.h"
#include "XCELReaderDataObject.h"
#include "XCELEncoding.h"

class XCELProcessorInterface;
class XCELParseTreeFactory;

class SymbolDoesNotMatchHandle
{
	public:
		virtual BOOL handle(XCLParsingExpression* expr, QByteArray* ba);
};

class SkipWhiteSpaceHandle : public SymbolDoesNotMatchHandle
{
	public:
		virtual BOOL handle(XCLParsingExpression* expr, QByteArray* ba);
};

/*! \class XCELFileParserState XCELFileParser.h "XCELFileParser.h"
 *  \brief Holds parsing states and filepositions to manage the processing of a file and document success or failure.
 
 * Used to perform jumps within the binary file by saving the current reading position into #startposition and continue reading there after the function returns. \n
 * Used to define parse ranges for items with "multiple" set to TRUE, which therefore have no specific length.
 
  @author Jan Schnasse <jan.schnasse@uni-koeln.de>
 */

class FileParserState
{
	private:
		void operator=(const FileParserState& st);
	public:
		//! Constructor. Initializes the current reading position for the file parser with 0 and the endposition with LENGTH_MAX.
		FileParserState()
		{
			startposition			= 0;
			endposition				= LENGTH_MAX;
			sDMhandleIsActive		= TRUE;
			isInDefinitionContext	= FALSE;
		}
		
		void addExpressionsToIndex(QVector<XCLParsingExpression*>* expressions);
		void addExpressionToIndex(XCLParsingExpression* expression);
		void removeExpressionsFromIndex( QVector <XCLParsingExpression*>* expressions);
	
	public:
		_LENGTH							endposition; //!< endposition for an item with multiple set to TRUE.
		_LENGTH							startposition;//!< the current parsing position
		XCLTree<XCLParsingExpression*>	index;//!< a Tree-index of all elements which have yet been read
		BOOL							sDMhandleIsActive;
		XCLParsingItem*					parent;
		XCLSyntaxElement*				actElement;
		BOOL							isInDefinitionContext;
};



class ReaderProperties
{
	public:
		enum ADDRSCHEME
		{
			BYTE = 0,
			SEPARATOR = 1
		};
		BOOL sDNMhandleIsActive;
		enum ADDRSCHEME addrScheme;
		QByteArray addressSeperator;
};


class ReaderData
{
	public:
		XCLSyntaxItem*							grammar;
		QHash<QString,XCLSyntaxExpression*>*	grammarIndex;
		XCLParsingItem*							parsedTree;
		QByteArray*								binaryArray;
		_LENGTH									binaryArrayLength;
};


/*! \class XCELFileParser XCELFileParser.h "XCELFileParser.h"
 *  \brief Processing of the binary file with respect to the grammar created by the XCELGrammarBuilder.
 
 
 
  @author Jan Schnasse <jan.schnasse@uni-koeln.de>
 */
class XCELReader
{
	public:
		XCELReader(XCELProcessorInterface* proc);
		~XCELReader();

		/*const XCLParsingItem& read(const XCLSyntaxItem& grammar,
                             QByteArray* ba,
                             XCLParsingItem* parseTree = NULL);
		*/
		
		const XCLParsingItem& parseFile(const QString& fileName,
			const XCLSyntaxItem& grammar);
		
		void setParseTreeFactory(XCLParseTreeFactory* factory);
		void setProcessor(XCELProcessorInterface* proc);
		XCELProcessorInterface* getProcessor()
		{
			return processor;
		}
		
		XCLSyntaxItem* getGrammar();
		
		QHash<QString,XCLSyntaxExpression*>* getGrammarIndex();
		void registerParsedValues(QVector<XCLParsingExpression*>* expressions);
		void registerParsedValues(XCLTree<XCLParsingExpression*>* i);
		
		QVector<XCLParsingExpression*>* parse( XCLSyntaxElement* grammItem,
			XCLParsingItem* parent,
			FileParserState& state);
		
		const XCLParsingItem& getDebugTree();
		
		FileParserState* getState();
		
		QByteArray* getBinaryArray();
		
		void setSymbolDoesNotMatchHandle(SymbolDoesNotMatchHandle* h);
		
		void setAddressingScheme(const QString& as, const QByteArray& addrSep = 0);
		
		void gotToLastAdress( );
		
		void goToPreviousAddress( );
		
		int goToNextAddress( );
		const QString& getFileName()
		{
			return fileName;
		}
		void setFileName(const QString& fn)
		{
			fileName=fn;
		}
	private:
		QVector<XCLParsingExpression*>* handleNextValue(XCLParsingSymbol* pExpr,
			XCLSyntaxSymbol* syntaxSymbol,
			FileParserState& state);
		
		QVector<XCLParsingExpression*>* parseItem(XCLSyntaxItem* grammItem,
			XCLParsingItem* parent,
			FileParserState& state);

		QVector<XCLParsingExpression*>* parseStructuringItem(XCLSyntaxItem* element,
			XCLParsingItem* parent,
			FileParserState& state);
		
		QVector<XCLParsingExpression*>* parseSemanticItem(XCLSyntaxItem* element,
			FileParserState& state);
		
		QVector<XCLParsingExpression*>* parseAllItem(	XCLSyntaxItem* syntaxItem,
			XCLParsingItem* parent,
			FileParserState& state);
		
		QVector<XCLParsingExpression*>* parseSequenceItem(XCLSyntaxItem* syntaxItem,
			XCLParsingItem* parent,
			FileParserState& state);
		
		QVector<XCLParsingExpression*>* parseChoiceItem(XCLSyntaxItem* syntaxItem,
			XCLParsingItem* parent,
			FileParserState& state);
		
		QVector<XCLParsingExpression*>* parseSymbol(	XCLSyntaxSymbol* syntaxSymbol,
			FileParserState& state);
		
		QVector<XCLParsingExpression*>* parseProperty(XCLSyntaxProperty* syntaxProperty,
			FileParserState& state);
		
		QVector<XCLParsingExpression*>* parseProcessing(XCLSyntaxProcess* element,
			XCLParsingItem* parent,
			FileParserState& state);
		
		QVector<XCLParsingExpression*>* parsePushXCELProcessing(XCLSyntaxProcess* element,
			FileParserState& state);
		
		QVector<XCLParsingExpression*>* parsePullXCELProcessing(XCLSyntaxProcess* element,
			XCLParsingItem* parent,
			FileParserState& state);
		
		void parsePushBinaryProcessing( XCLSyntaxProcess* element,
			QByteArray* binaryArray,
			FileParserState& state);
		
		BOOL setParsingValue(XCLParsingSymbol* symbol);
		
		BOOL parseMultiple(XCLSyntaxExpression* element,
			XCLParsingItem* parent,
			FileParserState& state,
			QVector <XCLParsingExpression*>& matchingExpressions);
		
		BOOL parseSimple(XCLSyntaxElement* element,
			XCLParsingItem* parent,
			FileParserState& state,
			QVector <XCLParsingExpression*>& matchingExpressions);
		
		void createBinaryArray(QString binFileName);
		
		void print(XCLSyntaxElement* element);
		
		void print(XCLParsingExpression* element);
		
		void print();

	private:
		XCLParseTreeFactory*		pFactory;//!< Object to call the XCLParseTreeFactory::create() and create the appropriate XCLParseTreeExpression for an XCLSyntaxItem.
		XCELProcessorInterface*		processor;
		QStack<_LENGTH>				positionStack;//!< For keeping track of the parsing positions in case of jumps
		QStack<QString>				sourceStack;
		QStack<QByteArray*>			internalSourceStack;
		QVector<_LENGTH>			addressVector;
		_LENGTH						addressPointer;
		FileParserState				mstate;
		XCLParsingItem*				debugTree;
		SymbolDoesNotMatchHandle*	sDNMhandle;
		ReaderProperties			props;
		ReaderData					data;
		QString						fileName;
		QString						actSource;
		XCELEncoding				encoder;
};



#endif
