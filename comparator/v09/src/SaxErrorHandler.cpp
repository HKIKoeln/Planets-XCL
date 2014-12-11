#include <iostream>
#include "ComparatorError.h"
#include "defines.h"
#include "SaxErrorHandler.h"

SaxErrorHandler :: SaxErrorHandler(ComparatorError* pError) {pErr= pError;}

SaxErrorHandler :: ~SaxErrorHandler() {}

inline void SaxErrorHandler :: warning(const SAXParseException& e)
{
 /* do nothing */
}
inline void SaxErrorHandler :: error(const SAXParseException& e)
{ 
char* s;
unsigned int lineNumber= e.getLineNumber();std::cout<<lineNumber;

//convert to string again:  
sprintf (s, "%u", lineNumber);
std::string result= std::string(s);      
std::cout<<result;     
pErr->registrateError("Error: SaxErrorHandler: File is not valid.", result); 

#ifdef DEBUG_VALIDATION  
std::cout<<"Error"<<std::endl;
std::cout<<"Line: " <<e.getLineNumber()<<std::endl;   
#endif    
}

inline void SaxErrorHandler :: fatalError(const SAXParseException& e) 
{
#ifdef DEBUG_VALIDATION
std::cout<<"Fatal Error"<<std::endl; 
std::cout<<"Line: " <<e.getLineNumber()<<std::endl;   
#endif             
}
