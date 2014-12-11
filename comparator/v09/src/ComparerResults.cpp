#include "defines.h"
#include "ComparerResults.h"

Results :: Results() : pRS(NULL), ResultBox(NULL)
{
}
Results :: ~Results() {cleanUp();}

void Results :: cleanUp()
{
if(!source.empty()) source.clear();
if(!target.empty()) target.clear();

std::map <std::string, ResultSet*> :: iterator i1;
if(ResultBox!=NULL)
  {
  if(!ResultBox->empty())
    {
    for(i1=ResultBox->begin(); i1!=ResultBox->end(); ++i1)
       {
       if(i1->second!=NULL) delete i1->second;
       }
    ResultBox->clear();
    }
  delete ResultBox; ResultBox= NULL;
  }

if(pRS!=NULL) pRS= NULL;
}

void Results :: printComparerResults()
{
std::map <std::string, ResultSet*> :: iterator i;
ResultSet *ptrRS;
std::string propName;

//std::cout<<std::endl<<"Comparison Results:"<<std::endl<<std::endl;

for(i=ResultBox->begin();i!=ResultBox->end();++i)
   {
   std::cout<<"Property: "<<i->first<<std::endl;
   ptrRS= i->second;
   ptrRS->printResultSet();
   }
}


Values :: Values() : srcValue(""), tarValue("")
{type.reserve(10);}

Values :: ~Values() {cleanUp();}

void Values :: cleanUp()
{
if(!type.empty()) type.clear();
if(!srcValue.empty()) srcValue.clear();
if(!tarValue.empty()) tarValue.clear();
}


ResultSet :: ResultSet() : pV(NULL), propMetrics(NULL), compStatus(0), errorcode("")
{
}
ResultSet :: ~ResultSet() {cleanUp();}

void ResultSet :: cleanUp()
{
if(propMetrics!=NULL)
  {
  if(!propMetrics->empty()) propMetrics->clear();
  delete propMetrics; propMetrics= NULL;
  }
if(pV!=NULL) {delete pV; pV=NULL;}
if(!propId.empty()) propId.clear();
}

void ResultSet :: printResultSet()
{
std::map <std::string, std::string> :: iterator i;

std::cout<<"Id: "<<propId<<std::endl;
std::cout<<"Values:"<<std::endl;
std::cout<<"Type: "<<pV->type<<std::endl;
std::cout<<"SrcValue: "<<pV->srcValue<<std::endl;
std::cout<<"TarValue: "<<pV->tarValue<<std::endl;

for(i=propMetrics->begin();i!=propMetrics->end();++i)
   {
   std::cout<<"Metric:"<<i->first<<"   Result:"<<i->second<<std::endl;
   }
std::cout<<std::endl;
}


