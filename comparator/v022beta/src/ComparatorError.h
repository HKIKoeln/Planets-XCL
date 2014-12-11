/*
    * This module is part of the XCL software system
    *
    * Written by  Sebastian Beyl, Volker Heydegger, Jan Schnasse,
    *             Manfred Thaller
    *             2007 - 2009
    *
    * Copyright (C)  2007 - 2009
    * Sebastian Beyl, Volker Heydegger, Jan Schnasse, Manfred Thaller
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

#ifndef _COMPARATORERROR_H
#define _COMPARATORERROR_H

#include <string>
#include <list>

class ComparatorError
{
public:
       ComparatorError();
       ~ComparatorError();

       void registrateError(std::string error);
       void registrateError(std::string error, std::string errorcode);
       void printLogfile();
       void cleanUp();

       int error;
       std::list <std::string> *errors;
       std::string cprName;
       std::string errorcode;
       std::string message;


};
#endif
