/***
    * This module is part of the XCL software system
    *
    * Copyright (C) 2007 - 2008 University of Cologne,
                    Historisch-Kulturwissenschaftliche Informationsverarbeitung
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
***/

#ifndef FPMUNITERROR_H
#define FPMUNITERROR_H

#include <string>

class FPMUnitError
{
      public:
             FPMUnitError() {}
             ~FPMUnitError() {}
             void registrateError(std::string errorcode, std::string option);
             void handleError(std::string errCode, std::string option);
             int getAction();
             std::string returnData;
      private:
             int action;
             std::string errCode;
};
#endif
