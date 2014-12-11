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
#include "Defines.h"
#include "XCLParsingItem.h"

#include <iostream>
#include <QString>
#include <QVector>

class XCLCalculator
{
    public:

        XCLCalculator();
        ~XCLCalculator();

        _UINT32 parseExpression(const QString& mathExpression, XCLTree<XCLParsingExpression*> index);

    private:
        QString& multiply(QString& operandOne, QString& operandTwo);
        QString add(QString& operandOne, QString& operandTwo);
        QString substract(QString& operandOne, QString& operandTwo);
        QString divide(QString& operandOne, QString& operandTwo);
        float getNumber(const QString& operandOne);

    private:
    
        QVector<QString> operands;
        XCLTree<XCLParsingExpression*> myIndex;


};

