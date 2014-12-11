#include <xercesc/sax2/Attributes.hpp>
#include "defines.h"
#include "XercesString.h"
#include "ComparatorError.h"
#include "Request2InHandler.h"
#include "Request2Input.h"
#include <string>
#include <map>


Request2InHandler :: Request2InHandler()
{
  parsingState= start;
  pRequest2= NULL;

  #ifdef DEBUG_REQUEST2
  std::cout<<"Request2InHandler... Initializing default object..."<<std::endl;
  #endif
}

Request2InHandler :: Request2InHandler(ComparatorError *pCE) : pRequest2(NULL)
{
parsingState= start;
pErr= pCE;
#ifdef DEBUG_REQUEST2
std::cout<<"Request2InHandler... Initializing std. object..."<<std::endl;
#endif
}

Request2InHandler :: ~Request2InHandler()
{
if(pRequest2!=NULL) {delete pRequest2; pRequest2= NULL;}
if(pErr!=NULL) pErr= NULL;
#ifdef DEBUG_RELEASE
std::cout<<"Release Request2InHandler obj ..."<<std::endl;
#endif
}


void Request2InHandler :: startElement(
    const XMLCh* const uri,
    const XMLCh* const localname,
    const XMLCh* const qname,
    const Attributes& attrs)
{
  std::string str1, str2, tmp;

  char *strLocalname = XMLString::transcode(localname);

  #ifdef DEBUG_REQUEST2
  printf("starting Element... %s\n", strLocalname);
  #endif

  if(parsingState==start)
    {
    const XercesString ELEM_pcRequest("pcRequest");

    if(!XMLString::compareString(localname, ELEM_pcRequest))
      {
      if(pRequest2==NULL)   pRequest2= new Request2();
      parsingState= startRequest2;

      #ifdef DEBUG_REQUEST2
      printf("found <pcRequest> Tag... %s\n", strLocalname);
      #endif
      }
    else
      {
      pErr->registrateError("Error: Request2InHandler: Expected <pcRequest> Tag at parsing state \'start\'.");
      }
    }
  else if(parsingState==startRequest2)
    {
    const XercesString ELEM_compSet("compSet");
    if(!XMLString::compareString(localname, ELEM_compSet))
      {
      // create new list object/ throw parsed compSet into list container:
      if(pRequest2->compSets==NULL) pRequest2->compSets= new std::list <MeasurementRequest*> ();
      else pRequest2->compSets->push_back(pRequest2->pMR);

      pRequest2->pMR= new MeasurementRequest();
      pRequest2->pMR->sets= new std::list <MetricSet*> ();
      parsingState=startMeasurementRequest;

      #ifdef DEBUG_REQUEST2
      printf("found <compSet> Tag... %s\n", strLocalname);
      #endif
      }
    else
      {
       pErr->registrateError("Error: Request2InHandler: Expected <compSet> Tag at parsing state \'startRequest2\'.");
      }
    }
  else if(parsingState==startMeasurementRequest)
    {
    const XercesString ELEM_property("property");

    if(!XMLString::compareString(localname, ELEM_property))
      {
      parsingState= startProperty;
      pRequest2->pMR->pMS= new MetricSet();

      // get attribute: id
      const XercesString ATTR_propID("id");
      const XercesString propID(attrs.getValue(ATTR_propID));
      char* id= XMLString::transcode(propID);
      str1= std::string(id);
      pRequest2->pMR->pMS->propertyID= str1;
      XMLString::release(&id);

      // get attribute: name
      const XercesString ATTR_propName("name");
      const XercesString propName(attrs.getValue(ATTR_propName));
      char* pn= XMLString::transcode(propName);
      str1= std::string(pn);
      pRequest2->pMR->pMS->propertyName= str1;
      XMLString::release(&pn);

      // get attribute: unit                                 UNIT is optional!!
      const XercesString ATTR_propUnit("unit");
      if(attrs.getValue(ATTR_propUnit))
        {
        const XercesString propUnit(attrs.getValue(ATTR_propUnit));
        char* pu= XMLString::transcode(propUnit);
        str1= std::string(pu);
        pRequest2->pMR->pMS->propertyUnit= str1;
        XMLString::release(&pu);
        }
      else
        {
        pRequest2->pMR->pMS->propertyUnit="-1";
        }
      /*
      std::cout<<pRequest2->pMR->pMS->propertyID<<std::endl;
      std::cout<<pRequest2->pMR->pMS->propertyName<<std::endl;
      std::cout<<pRequest2->pMR->pMS->propertyUnit<<std::endl;
      */
      }
    else
      {
      const XercesString ELEM_source("source");
      const XercesString ELEM_target("target");

      // get attribute: name
      const XercesString ATTR_name("name");
      const XercesString name(attrs.getValue(ATTR_name));
      char* n= XMLString::transcode(name);
      str1= std::string(n);

      if(!XMLString::compareString(localname, ELEM_source))
        {
        pRequest2->pMR->source= str1;
        XMLString::release(&n);
        }
      else if(!XMLString::compareString(localname, ELEM_target))
        {
        pRequest2->pMR->target= str1;
        XMLString::release(&n);
        }
      else
        {
        XMLString::release(&n);
        pErr->registrateError("Error: Request2InHandler: Unexpected element in tag <compSet>.");
        //parsingState= erroneous;
        }
      }
    }
  else if(parsingState==startProperty)
    {
    const XercesString ELEM_metric("metric");

    if(!XMLString::compareString(localname, ELEM_metric))
      {
      // generate new metric list container, if not done so far:
      if(pRequest2->pMR->pMS->metricSets==NULL)
        {
        pRequest2->pMR->pMS->metricSets= new std::map <std::string, std::string> ();
        }
      // get attribute: id
      const XercesString ATTR_metricID("id");
      const XercesString metricID(attrs.getValue(ATTR_metricID));
      char* mid= XMLString::transcode(metricID);
      str1= std::string(mid);

      // get attribute: name
      const XercesString ATTR_metricName("name");
      const XercesString metricName(attrs.getValue(ATTR_metricName));
      char* mname= XMLString::transcode(metricName);
      str2= std::string(mname);

      // get attribute: param                    OPTIONAL!
      const XercesString ATTR_param("param");
      if(attrs.getValue(ATTR_param))
        {
        const XercesString param(attrs.getValue(ATTR_param));
        char* prm= XMLString::transcode(param);
        tmp= std::string(prm);
        str2= str2 + "#" + tmp;
        }
      // store to Metrics Container
      pRequest2->pMR->pMS->metricSets->insert(make_pair(str1, str2));
      XMLString::release(&mid);
      XMLString::release(&mname);
      }
    else
      {
      pErr->registrateError("Error: Request2InHandler: Expected element <metric> in parsing state \'startProperty\'.");
      //parsingState= erroneous;
      }
    }
XMLString::release(&strLocalname);
}


void Request2InHandler :: endElement(const XMLCh* const uri,
                                     const XMLCh* const localname,
                                     const XMLCh* const qname)
{
char *strLocalname = XMLString::transcode(localname);

const XercesString ENDELEM_property("property");
const XercesString ENDELEM_compSet("compSet");
const XercesString ENDELEM_pcRequest("pcRequest");

if(!XMLString::compareString(localname, ENDELEM_property))
  {
  parsingState= startMeasurementRequest;
  pRequest2->pMR->sets->push_back(pRequest2->pMR->pMS);
  }
else if(!XMLString::compareString(localname, ENDELEM_compSet))
  {
  parsingState= startRequest2;
  }
else if(!XMLString::compareString(localname, ENDELEM_pcRequest))
  {
  pRequest2->compSets->push_back(pRequest2->pMR);  // throw compSet into list
  parsingState= endRequest2;
  }

XMLString::release(&strLocalname);
}



