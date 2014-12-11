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


#ifndef XCLPARSINGSYMBOL_H
#define XCLPARSINGSYMBOL_H
#include "XCLParsingExpression.h"

#include <QString>
#include <QHash>
#include <QVector>

class XCLParsingSymbol : public XCLParsingExpression
{
	public:// Constructors/Destructors
		XCLParsingSymbol ( );
		virtual ~XCLParsingSymbol ( );
		XCLParsingSymbol(const XCLParsingSymbol& s);
		XCLParsingSymbol& operator=(const XCLParsingSymbol& s);
	
	protected://members
		QByteArray value;
		// QString parsedValue;
		QString interpretedValue;
		QString interpretation;
		QHash<QString,QString> semanticInterpretation;
		QVector<QString> validValues;
		QVector<QString> nonValidValues;
		_SIZE privateLength;
		QString compareValue;
		BOOL isValidFlag;

	protected:
		const QVector<QString>& getValidValues ( );
		const QString& getFixedLabel();
		BOOL compare(const QString& strOrPat,const QString& str2);
		QString encoding;

	public://inherited
		const QByteArray& getValue ( ) ;
		enum EXPR_TYPE getType();
		// const QString& getParsedValue ( );
		void print(QTextStream& debugOut, QString format="");
		void print(QString format="");
		XCLParsingSymbol* getFirstProperty();

	public://virtual
		virtual const QString& getInterpretedValue ( ) ;
		virtual void setInterpretedValue(const QString& v);
		virtual BOOL isValid();

	public:
		void setEncoding ( const QString& new_var );
		const QString& getEncoding ();
		
		_LENGTH getLength(QByteArray* array);
		_LENGTH getPrivateLength();
		void setCompareValue(const QByteArray& cV);
		const QString& getCompareValue();
		void setValue (const QByteArray& new_var);
		
		void setValidValues ( const QVector<QString>& new_var );
		void setNonValidValues ( const QVector<QString>& new_var );

		const QString& getInterpretation ( );
		void setInterpretation ( const QString& new_var );

		void setSemanticInterpretation (const  QHash<QString,QString>& new_var );
		const QHash<QString,QString>& getSemanticInterpretation ( );

	private:
		BOOL checkValidValues();
		BOOL checkNonValidValues();
		void initAttributes ( ) ;

};

#endif // XCLPARSINGSYMBOL_H
