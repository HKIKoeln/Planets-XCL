#include <list>
#include "defines.h"
#include "Request2Input.h"

Request2 :: Request2() : pMR(NULL), compSets(NULL)
{
}

Request2 :: ~Request2()
{
cleanUp();
}

void Request2 :: printRequest2Input()
{
std::list <MeasurementRequest*> :: iterator i1;
std::list <MetricSet*> :: iterator i2;
MeasurementRequest *pMReq;
MetricSet *ptrMS;
std::string str;

for(i1=compSets->begin();i1!=compSets->end();++i1)
   {
   pMReq= *i1;
   std::cout<<"Request2 Input:"<<std::endl;
   std::cout<<"Source: "<<pMReq->source<<std::endl;
   std::cout<<"Target: "<<pMReq->target<<std::endl;
   if(pMReq->ok== false) std::cout<<"ok: unmatched compSet"<<std::endl;
   else std::cout<<"ok: matched compSet"<<std::endl;

   for(i2=pMReq->sets->begin();i2!=pMReq->sets->end();++i2)
      {
      ptrMS=*i2;
      std::cout<<"Property:"<<std::endl;
      std::cout<<ptrMS->propertyName<<std::endl;
      std::cout<<ptrMS->propertyID<<std::endl;
      ptrMS->printMetrics();
      }
   }
}

void Request2 :: cleanUp()
{
std::list <MeasurementRequest*> :: iterator i1;

if(compSets!=NULL)
  {
  if(!compSets->empty())
    {
    for(i1=compSets->begin();i1!=compSets->end();++i1)
       {
       if(*i1!=NULL) delete *i1;
       }
    compSets->clear();
    }
  delete compSets; compSets= NULL;
  }
if(pMR!=NULL) pMR= NULL;

#ifdef DEBUG_RELEASE
std::cout<<"Released Request2 obj ..."<<std::endl;
#endif
}

MeasurementRequest :: MeasurementRequest() : pMS(NULL), sets(NULL), ok(true)
{
}

MeasurementRequest :: ~MeasurementRequest()
{
cleanUp();
}


void MeasurementRequest :: cleanUp()
{
std::list <MetricSet*> :: iterator i1;

if(sets!=NULL)
  {
  if(!sets->empty())
    {
    for(i1=sets->begin();i1!=sets->end();++i1)
      {
      if(*i1!=NULL) delete *i1;
      }
    sets->clear();
    }
  delete sets; sets=NULL;
  }

if(pMS!=NULL) pMS= NULL;

if(!source.empty()) source.clear();
if(!target.empty()) target.clear();
#ifdef DEBUG_RELEASE
std::cout<<"Released MeasurementRequest obj..."<<std::endl;
#endif
}

MetricSet :: MetricSet() : metricSets(NULL)
{
}

MetricSet :: ~MetricSet()
{
cleanUp();
}

void MetricSet :: cleanUp()
{
if(metricSets!=NULL)
  {
  if(!metricSets->empty()) metricSets->clear();
  delete metricSets; metricSets= NULL;
  }

if(!propertyName.empty()) propertyName.clear();
if(!propertyID.empty()) propertyID.clear();
if(!propertyUnit.empty()) propertyUnit.clear();
#ifdef DEBUG_RELEASE
std::cout<<"Released MetricSet obj ..."<<std::endl;
#endif
}

void MetricSet :: printMetrics()
{
std::map <std::string, std::string> :: iterator i1;
int a;

for(a=1,i1=metricSets->begin(); i1!=metricSets->end(); ++i1, ++a)
  {
  std::cout<<"Metric #"<<a<<":"<<"  ID:"<<i1->first<<"  Name:"<<i1->second<<std::endl;
  }
std::cout<<std::endl;
}


