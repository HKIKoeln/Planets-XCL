#include "defines.h"
#include "PropertyTagset.h"

ValueSet :: ValueSet(): valueSetID("\0"), rawVal("\0"), labVal(false), labValVal("\0"),
                         labValType("\0"), dataRef(false), propRel(false)
{
}

ValueSet :: ~ValueSet() {}

void ValueSet :: cleanUp()
{
if(!valueSetID.empty()) valueSetID.clear();
if(!rawVal.empty()) rawVal.clear();
if(!labValVal.empty()) labValVal.clear();
if(!labValType.empty()) labValType.clear();
printValueSet();
}

void ValueSet :: printValueSet()
{
#ifdef DEBUG_VALUESET
std::cout<<"ValueSet Values:"<<std::endl;
std::cout<<"valueSetID= "<<valueSetID<<std::endl;
std::cout<<"rawVal= "<<rawVal<<std::endl;
if(labVal==true) std::cout<<"labVal= true"<<std::endl;
else std::cout<<"labVal= false"<<std::endl;
std::cout<<"labValVal= "<<labValVal<<std::endl;
std::cout<<"labValType= "<<labValType<<std::endl;
if(dataRef==true) std::cout<<"dataRef= true"<<std::endl;
else std::cout<<"dataRef= false"<<std::endl;
if(propRel==true) std::cout<<"propRel= true"<<std::endl;
else std::cout<<"propRel= false"<<std::endl;
system("PAUSE");
#endif
}


PropertyTagset :: PropertyTagset() : propName("\0"), propID("\0"), valueSets(NULL), pValueSet(NULL)
{
}

PropertyTagset :: ~PropertyTagset()
{
cleanUp();
}

void PropertyTagset :: cleanUp()
{
std::map <std::string, ValueSet*> :: iterator i1;

if(valueSets!=NULL)
  {
  if(!valueSets->empty())
    {
    for(i1=valueSets->begin(); i1!=valueSets->end(); ++i1)
       {
       if(i1->second!=NULL) {i1->second->cleanUp(); delete i1->second;}
       }
    valueSets->clear();
    }
  delete valueSets; valueSets= NULL;
  }

if(pValueSet!=NULL) {pValueSet= NULL;}  // Achtung: nur auf NULL setzen, da Zeiger auf Speicher verwweist, der vorher freigegeben wurde!!
if(!propName.empty()) propName.clear();
#ifdef DEBUG_RELEASE
std::cout<<"Released objects class PropertyTagset..."<<std::endl;
#endif
}

