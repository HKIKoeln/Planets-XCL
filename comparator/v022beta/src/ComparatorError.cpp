#include <string>
#include <fstream>
#include "defines.h"
#include "timestamp.h"
#include "ComparatorError.h"

ComparatorError :: ComparatorError() : cprName("\0"), error(0),
                                       errorcode("\0"), message("\0")
{
errors= new std::list <std::string> ();
}

ComparatorError :: ~ComparatorError()
{
cleanUp();
}

void ComparatorError :: cleanUp()
{
if(errors!=NULL)
  {
  if(!errors->empty()) errors->clear();
  delete errors;
  errors= NULL;
  }

if(!cprName.empty()) cprName.clear();
if(!message.empty()) message.clear();
if(!errorcode.empty()) errorcode.clear();
}

void ComparatorError :: registrateError(std::string error)
{
errors->push_back(error);
}

// overloaded version for attribute error in <metric> tag
void ComparatorError :: registrateError(std::string error,std::string errcode)
{
errors->push_back(error);
errorcode= errcode;
}

void ComparatorError :: printLogfile()
{
std::list <std::string> :: iterator i;

std::ofstream logOf("log.txt", std::ios::app);

logOf<<"Process time: "<<timestring()<<" \n";

if(!errors->empty())
  {
  for(i=errors->begin();i!=errors->end();++i)
     {
     logOf<<*i<<std::endl;
     }
  }
else logOf<<"No errors occured."<<std::endl;

logOf<<std::endl;

logOf.close();
}



