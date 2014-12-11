/*****************************************************************************
dataTagset.h

Description:
*****************************************************************************/
#ifndef DATATAGSET_H
#define DATATAGSET_H

#include <string>
#include <map>
#include <vector>

class NormData
{
   public:
         NormData();
         ~NormData();
         void cleanUp();
         void releaseNormDataTagsetsUint();
         void releaseNormDataTagsetsStr();
         void printNormdata();  // for debugging
         inline void storeNormDataAsIs(char* normData) {normDataAsIs= std::string(normData);}
         void loadNormDataAsStr(NormData *pND, char* normdata);
         void loadNormDataAsUint(NormData *pND, char* normdata);

         std::map <std::string, std::vector <std::string>* > *normDataTagsetsStr;
         std::map <std::string, std::vector <int>* > *normDataTagsetsUint;
         std::vector <std::string> *normDataAsStr;
         std::vector <int> *normDataAsUint;
         std::string normDataAsIs;
         std::string normDataType;
         std::string key;
};

class RawData
{
   public:
         RawData();
         ~ RawData();
};
#endif
