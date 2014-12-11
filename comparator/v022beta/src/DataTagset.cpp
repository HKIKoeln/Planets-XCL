#include <string>
#include <map>
#include <vector>
#include <sstream>
#include "defines.h"
#include "DataTagset.h"

NormData :: NormData() : normDataTagsetsUint(NULL), normDataAsUint(NULL),
                         normDataTagsetsStr(NULL), normDataAsStr(NULL),
                         normDataAsIs("\0"),key("\0"), normDataType("\0")
{
}

NormData :: ~NormData() {}

void NormData :: cleanUp()
{
releaseNormDataTagsetsStr();
releaseNormDataTagsetsUint();
if(!normDataAsIs.empty()) normDataAsIs.clear();
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

if(normDataAsUint!=NULL)
  {
  normDataAsUint= NULL;
  }
#ifdef DEBUG_RELEASE
std::cout<<"Released normDataTagsetsUint obj..."<<std::endl;
//system("PAUSE");
#endif
return;
}

void NormData :: releaseNormDataTagsetsStr()
{
std::map <std::string, std::vector <std::string>* > :: iterator i1;
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

if(normDataAsStr!=NULL)
  {
  normDataAsStr= NULL;
  }
#ifdef DEBUG_RELEASE
std::cout<<"Released normDataTagsetsStr obj..."<<std::endl;
//system("PAUSE");
#endif
return;
}

void NormData :: loadNormDataAsStr(NormData *pND, char* normData)
{
std::string str;
std::istringstream iss (normData);

for(int i =0;iss.good();++i)
   {
   iss >> str >> std::ws;
   pND->normDataAsStr->push_back(str);
   }

pND->normDataTagsetsStr->insert(make_pair(pND->key, pND->normDataAsStr));
}

void NormData :: loadNormDataAsUint(NormData *pND, char* normData)
{
int val;
std::istringstream iss (normData);

for(int i =0;iss.good();++i)
   {
   iss >> std::hex >> val >> std::ws;
   pND->normDataAsUint->push_back(val);
   }
//std::cout<<"NormData has been loaded..."<<std::endl;
pND->normDataTagsetsUint->insert(make_pair(pND->key, pND->normDataAsUint));
}


RawData :: RawData() {}
RawData :: ~RawData() {}

