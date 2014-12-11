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
#include "XCLCalculator.h"
#include <QTextStream>

XCLCalculator::XCLCalculator()
{

};

XCLCalculator::~XCLCalculator()
{

};

_UINT32 XCLCalculator::parseExpression(const QString& mathExpression, XCLTree<XCLParsingExpression*> index)
{
    myIndex = index;
    QString mathEx;
    QString operand = QString(); 

    for (int i =0;i<mathExpression.size();i++)
        if ( mathExpression[i]!=QChar(32)) //remove all whitespace
            mathEx.append(mathExpression[i]);

    //break parseString into different operands; mathmatical operators are saves with the preceeding operand

    while(mathEx.indexOf("*")!=-1 || mathEx.indexOf("/")!=-1 || mathEx.indexOf("+")!=-1 || mathEx.indexOf("-")!=-1)
    {
        for(int j=0;j<mathEx.length();j++)
        {
            operand.append(mathEx[j]);
            if (mathEx[j] == '*' || mathEx[j] == '/' || mathEx[j] == '+' || mathEx[j] == '-' )
                break; 
        }

        operands.append(operand); //put operand on operands list.
        mathEx = mathEx.remove(0,operand.size()); //remove parsed operand from expression string
    }
    operands.append(mathEx);

    try
    {
	//loop for multiplications and divisions
	int i =0;
	while ( (operands.size()>1) && (i< operands.size()) )
	{
	    if (operands[i].contains("*"))
	    {
		
		operands[i] = multiply(operands[i],operands[i+1]);
		operands.remove(i+1);
	    }
	    else if(operands[i].contains("/"))
	    {
		
		operands[i] = divide(operands[i],operands[i+1]);
		operands.remove(i+1);
	    }
	    if (operands.size()==1)
		return operands[0].toInt();
	    i++;
	}
	
	//loop for addition and substraction
	i=0;
	while(operands.size()>1)
	{
	    if (operands[i].contains("+"))
	    {
	    operands[i] = add(operands[i],operands[i+1]);
	    operands.remove(i+1);
	    i=0;
	    }
	    else if (operands[i].contains("-"))
	    {
	    operands[i] = substract(operands[i],operands[i+1]);
	    operands.remove(i+1);
	    i=0;
	    }
    
	    if (operands.size()==1)
		return operands[0].toInt();
	}
	
    }
    catch(XCLException exception)
    {
        exception.message();
        throw XCLException("couldn't resolve complete Exprssion\n");
    }
    return -1;
};

QString& XCLCalculator::multiply(QString& operandOne, QString& operandTwo)
{
float floatOperandOne;
float floatOperandTwo;
QString mathOperator;
  
    //strip operator from first operand
    operandOne.remove(operandOne.indexOf("*"),1);


    //strip possible operators from second operand
    if((operandTwo.indexOf("+")!=-1)||(operandTwo.indexOf("-")!=-1)||(operandTwo.indexOf("*")!=-1)||(operandTwo.indexOf("/")!=-1))
    {
        mathOperator = operandTwo.right(1);
      
        operandTwo = operandTwo.remove(operandTwo.indexOf(" "),1);
    }
    
    //resolve any references 
    if(operandOne.startsWith("ID"))
    {
        XCLParsingExpression* expr = myIndex.get(operandOne)->last();
        operandOne = expr->getInterpretedValue();
   
    }
    if(operandTwo.startsWith("ID"))
    {
        XCLParsingExpression* expr = myIndex.get(operandTwo)->last();
        operandTwo = expr->getInterpretedValue();
    
    }

    //convert operators to floats
    floatOperandOne = operandOne.toFloat();
    floatOperandTwo = operandTwo.toFloat();
   

    try
    {
	QString* result = new QString();  
	QTextStream in(result,QIODevice::ReadOnly);
	in<<(floatOperandOne * floatOperandTwo);
	result = in.string();
	
        //reattach mathematical operator if more operations are to follow
        if (!mathOperator.isNull())
            result->append(mathOperator);

        return *result;
    }
    catch(XCLException exception)
    {
        exception.message();
        throw XCLException("couldn't calculate a valid value\n");
    }

};

QString XCLCalculator::add(QString& operandOne, QString& operandTwo)
{
float floatOperandOne;
float floatOperandTwo;
QString mathOperator;
  
    //std::cout<<"operandOne: "<<operandOne.toStdString()<<"operandTwo: "<<operandTwo.toStdString()<<std::endl;  

    floatOperandOne = getNumber(operandOne);
    //intOperandOne = operandOne.remove(operandOne.indexOf("+"),1).toInt();

    if((operandTwo.indexOf("+")!=-1)||(operandTwo.indexOf("-")!=-1)||(operandTwo.indexOf("*")!=-1)||(operandTwo.indexOf("/")!=-1))
    {
        mathOperator = operandTwo.right(1);
        //std::cout<<"math Operator: "<<mathOperator.toStdString()<<std::endl;
        floatOperandTwo = getNumber(operandTwo);

    //std::cout<<"operandOne: "<<floatOperandOne<<"operandTwo: "<<floatOperandTwo<<std::endl;  
        //intOperandTwo = operandTwo.remove(operandTwo.indexOf(" "),1).toInt();
        //intOperandTwo = operandTwo.toInt();
    }
    else
        floatOperandTwo = operandTwo.toInt();

    //std::cout<<"operandOne: "<<floatOperandOne<<"operandTwo: "<<floatOperandTwo<<std::endl;  

    QString* result = new QString();  
    QTextStream in(result,QIODevice::ReadOnly);

    in<<(floatOperandOne + floatOperandTwo);
    result = in.string();


    if (!mathOperator.isNull())
        result->append(mathOperator);

    return *result;
};

QString XCLCalculator::substract(QString& operandOne, QString& operandTwo)
{
float floatOperandOne;
float floatOperandTwo;
QString mathOperator;
  
    //std::cout<<"operandOne: "<<operandOne.toStdString()<<"operandTwo: "<<operandTwo.toStdString()<<std::endl;  

    floatOperandOne = getNumber(operandOne);

    if((operandTwo.indexOf("+")!=-1)||(operandTwo.indexOf("-")!=-1)||(operandTwo.indexOf("*")!=-1)||(operandTwo.indexOf("/")!=-1))
    {
        mathOperator = operandTwo.right(1);
      //  std::cout<<"math Operator: "<<mathOperator.toStdString()<<std::endl;
        floatOperandTwo = getNumber(operandTwo);

    //std::cout<<"operandOne: "<<floatOperandOne<<"operandTwo: "<<floatOperandTwo<<std::endl;  
    }
    else
        floatOperandTwo = operandTwo.toInt();

    //std::cout<<"operandOne: "<<floatOperandOne<<"operandTwo: "<<floatOperandTwo<<std::endl;  

    QString* result = new QString();  
    QTextStream in(result,QIODevice::ReadOnly);

    in<<(floatOperandOne - floatOperandTwo);
    result = in.string();


    if (!mathOperator.isNull())
        result->append(mathOperator);

    return *result;
};

QString XCLCalculator::divide(QString& operandOne, QString& operandTwo)
{
float floatOperandOne;
float floatOperandTwo;
QString mathOperator;
  
    //std::cout<<"operandOne: "<<operandOne.toStdString()<<"operandTwo: "<<operandTwo.toStdString()<<std::endl;  

    floatOperandOne = operandOne.remove(operandOne.indexOf("/"),1).toFloat();

    if((operandTwo.indexOf("+")!=-1)||(operandTwo.indexOf("-")!=-1)||(operandTwo.indexOf("*")!=-1)||(operandTwo.indexOf("/")!=-1))
    {
        mathOperator = operandTwo.right(1);
      //  std::cout<<"math Operator: "<<mathOperator.toStdString()<<std::endl;
        floatOperandTwo = operandTwo.remove(operandTwo.indexOf(" "),1).toFloat();
        //intOperandTwo = operandTwo.toInt();
    }
    else
        floatOperandTwo = operandTwo.toInt();

    QString* result = new QString();  
    QTextStream in(result,QIODevice::ReadOnly);

    in<<(floatOperandOne / floatOperandTwo);
    result = in.string();


    if (!mathOperator.isNull())
        result->append(mathOperator);

    return *result;
};

float XCLCalculator::getNumber(const QString& operandOne)
{
    QString operand = operandOne;
    if (operandOne.indexOf("+"))
        operand = operand.remove(operandOne.indexOf("+"),1);
    else if(operandOne.indexOf("-"))
        operand = operand.remove(operandOne.indexOf("-"),1);
    else if(operandOne.indexOf("*"))
        operand = operand.remove(operandOne.indexOf("*"),1);
    else if(operandOne.indexOf("/"))
        operand = operand.remove(operandOne.indexOf("/"),1);

    if (operand.startsWith("IDI"))
        {
            XCLParsingExpression* myEx = myIndex.get(operand)->at(0);
            operand = myEx->getInterpretedValue();
            std::cout<<"operand: "<<operand.toStdString()<<std::endl;
        }
    return operand.toFloat();
}


