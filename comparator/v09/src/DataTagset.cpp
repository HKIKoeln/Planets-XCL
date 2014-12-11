#include <string>
#include <map>
#include <vector>
#include <sstream>
#include "defines.h"
#include "DataTagset.h"

NormData :: NormData() : normDataTagsetsUint(NULL), normDataAsInt(NULL),
                         normDataTagsetsStr(NULL), normDataAsStrings(NULL),
                         normDataAsStdStr("\0"),key("\0"), normDataType("\0")
{
}

NormData :: ~NormData() {}

void NormData :: cleanUp()
{
releaseNormDataTagsetsStr();
releaseNormDataTagsetsUint();
if(!normDataAsStdStr.empty()) normDataAsStdStr.clear();
if(!normDataType.empty()) normDataType.clear();
if(!key.empty()) key.clear();

#ifdef DEBUG_RELEASE
std::cout<<"Released objects from class NormData..."<<std::endl;
//system("PAUSE");
#endif
return;
}

void NormData :: releaseNormDataTagsetsUint()
{
std::map <std::string, std::vector <int>* > :: iterator i2;

if(normDataTagsetsUint!=NULL)
  {
  if(!normDataTagsetsUint->empty())
    {
    for(i2=normDataTagsetsUint->begin(); i2!=normDataTagsetsUint->end(); ++i2)
       {
       if(i2->second!=NULL)
         {
         i2->second->clear();
         delete i2->second;
         }
       }
    normDataTagsetsUint->clear();
    }
  delete normDataTagsetsUint; normDataTagsetsUint= NULL;
  }

if(normDataAsInt!=NULL)
  {
  normDataAsInt= NULL;
  }
#ifdef DEBUG_RELEASE
std::cout<<"Released normDataTagsetsUint obj..."<<std::endl;
#endif
return;
}

void NormData :: releaseNormDataTagsetsStr()
{
std::map <std::string, std::vector <char>* > :: iterator i1;
if(normDataTagsetsStr!=NULL)
  {
  if(!normDataTagsetsStr->empty())
    {
    for(i1=normDataTagsetsStr->begin(); i1!=normDataTagsetsStr->end(); ++i1)
       {
       if(i1->second!=NULL)
         {
         i1->second->clear();
         delete i1->second;
         }
       }
    normDataTagsetsStr->clear();
    }
  delete normDataTagsetsStr; normDataTagsetsStr= NULL;
  }

if(normDataAsStrings!=NULL)
  {
  normDataAsStrings= NULL;
  }
#ifdef DEBUG_RELEASE
std::cout<<"Released normDataTagsetsStr obj..."<<std::endl;
//system("PAUSE");
#endif
return;
}

void NormData :: loadNormDataAsStrings()
{
std::string str;
std::istringstream iss (normDataAsIs);

if(normDataAsStrings==NULL) normDataAsStrings= new std::vector <std::string> ();

for(int i =0;iss.good();++i)
   {
   iss >> str >> std::ws;
   normDataAsStrings->push_back(str);
   }

//if(pND->normDataTagsetsStr==NULL) pND->normDataTagsetsStr= new std::map <std::string, std::vector <std::string>* > ();
//pND->normDataTagsetsStr->insert(make_pair(property, pND->normDataAsStr));

#ifdef DEBUG_NORMDATA
std::cout<<"NormData has been loaded as single strings..."<<std::endl;
#endif
}

/*
void NormData :: loadNormDataAsUint(std::string property, NormData *pND, char* normData)
{
int val;
std::istringstream iss (normData);

if(pND->normDataAsUint==NULL) pND->normDataAsUint= new std::vector <int> ();
for(int i =0;iss.good();++i)
   {
   iss >> std::hex >> val >> std::ws;
   pND->normDataAsUint->push_back(val);
   }
//std::cout<<"NormData has been loaded..."<<std::endl;
pND->normDataTagsetsUint->insert(make_pair(property, pND->normDataAsUint));
}
*/
void NormData :: loadNormDataAsInt(std::string property, NormData *pND, char* normData)
{
int val;
std::istringstream iss (normData);

if(pND->normDataAsInt==NULL) pND->normDataAsInt= new std::vector <int> ();

for(int i =0;iss.good();++i)
   {
   iss >> std::hex >> val >> std::ws;
   pND->normDataAsInt->push_back(val);
   }

if(pND->normDataTagsetsUint==NULL) pND->normDataTagsetsUint= new std::map <std::string, std::vector <int>* > ();
pND->normDataTagsetsUint->insert(make_pair(property, pND->normDataAsInt));

#ifdef DEBUG_NORMDATA
std::cout<<"NormData has been loaded..."<<std::endl;
#endif
}



RawData :: RawData() {}
RawData :: ~RawData() {}

