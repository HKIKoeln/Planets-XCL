#include <string>
#include <map>
#include <xercesc/sax2/Attributes.hpp>
#include "defines.h"
#include "XercesString.h"
#include "ComparatorError.h"
#include "ComparisonConfigurationHandler.h"
#include "ComparisonConfigurationData.h"


ComparisonConfigurationHandler :: ComparisonConfigurationHandler()
{
  parsingState= start;
  pCoco= NULL;

  #ifdef DEBUG_ComparisonConfiguration
  std::cout<<"ComparisonConfigurationHandler... Initializing default object..."<<std::endl;
  #endif
}

ComparisonConfigurationHandler :: ComparisonConfigurationHandler(ComparatorError *pCE) : pCoco(NULL)
{
parsingState= start;
pErr= pCE;
#ifdef DEBUG_ComparisonConfiguration
std::cout<<"ComparisonConfigurationHandler... Initializing std. object..."<<std::endl;
#endif
}

ComparisonConfigurationHandler :: ~ComparisonConfigurationHandler()
{
if(pCoco!=NULL) {delete pCoco; pCoco= NULL;}
if(pErr!=NULL) pErr= NULL;
#ifdef DEBUG_RELEASE
std::cout<<"Release ComparisonConfigurationHandler obj ..."<<std::endl;
#endif
}


void ComparisonConfigurationHandler :: startElement(
    const XMLCh* const uri,
    const XMLCh* const localname,
    const XMLCh* const qname,
    const Attributes& attrs)
{
  std::string str1, str2, tmp;

  char *strLocalname = XMLString::transcode(localname);

  #ifdef DEBUG_ComparisonConfiguration
  printf("starting Element... %s\n", strLocalname);
  #endif

  if(parsingState==start)
    {
    const XercesString ELEM_coco("coco");

    if(!XMLString::compareString(localname, ELEM_coco))
      {
      if(pCoco==NULL)   pCoco= new ComparisonConfiguration();
      parsingState= startComparisonConfiguration;

      #ifdef DEBUG_ComparisonConfiguration
      printf("found <coco> Tag... %s\n", strLocalname);
      #endif
      }
    else
      {
      pErr->registrateError("Error: ComparisonConfigurationHandler: Expected <coco> Tag at parsing state \'start\'.");
      std::cout<<"Warning: ComparisonConfigurationHandler: Expected <coco> Tag at parsing state \'start\'"<<std::endl;
      }
    }
  else if(parsingState==startComparisonConfiguration)
    {
    const XercesString ELEM_compSet("compSet");
    if(!XMLString::compareString(localname, ELEM_compSet))
      {
      // create new list object/ throw parsed compSet into list container:
      if(pCoco->compSets==NULL) pCoco->compSets= new std::list <CompSet*> ();
      else pCoco->compSets->push_back(pCoco->pCompSet);

      pCoco->pCompSet= new CompSet();
      pCoco->pCompSet->sets= new std::list <MetricSet*> ();
      parsingState=startCompSet;

      #ifdef DEBUG_ComparisonConfiguration
      printf("found <compSet> Tag... %s\n", strLocalname);
      #endif
      }
    else
      {
       pErr->registrateError("Error: ComparisonConfigurationHandler: Expected <compSet> Tag at parsing state \'startComparisonConfiguration\'.");
      }
    }
  else if(parsingState==startCompSet)
    {
    const XercesString ELEM_property("property");

    if(!XMLString::compareString(localname, ELEM_property))
      {
      parsingState= startProperty;
      pCoco->pCompSet->pMS= new MetricSet();

      // get attribute: id
      const XercesString ATTR_propID("id");
      const XercesString propID(attrs.getValue(ATTR_propID));
      char* id= XMLString::transcode(propID);
      str1= std::string(id);
      pCoco->pCompSet->pMS->propertyID= str1;
      XMLString::release(&id);

      // get attribute: name
      const XercesString ATTR_propName("name");
      const XercesString propName(attrs.getValue(ATTR_propName));
      char* pn= XMLString::transcode(propName);
      str1= std::string(pn);
      pCoco->pCompSet->pMS->propertyName= str1;
      XMLString::release(&pn);
      }
    }
  else if(parsingState==startProperty)
    {
    const XercesString ELEM_metric("metric");

    if(!XMLString::compareString(localname, ELEM_metric))
      {
      // generate new metric list container, if not done so far:
      if(pCoco->pCompSet->pMS->metricSets==NULL)
        {
        pCoco->pCompSet->pMS->metricSets= new std::map <std::string, std::string> ();
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
      pCoco->pCompSet->pMS->metricSets->insert(make_pair(str1, str2));
      XMLString::release(&mid);
      XMLString::release(&mname);
      }
    else
      {
      pErr->registrateError("Error: ComparisonConfigurationHandler: Expected element <metric> in parsing state \'startProperty\'.");
      //parsingState= erroneous;
      }
    }
XMLString::release(&strLocalname);
}


void ComparisonConfigurationHandler :: endElement(const XMLCh* const uri,
                                     const XMLCh* const localname,
                                     const XMLCh* const qname)
{
char *strLocalname = XMLString::transcode(localname);

const XercesString ENDELEM_property("property");
const XercesString ENDELEM_compSet("compSet");
const XercesString ENDELEM_coco("coco");

if(!XMLString::compareString(localname, ENDELEM_property))
  {
  parsingState= startCompSet;
  pCoco->pCompSet->sets->push_back(pCoco->pCompSet->pMS);
  }
else if(!XMLString::compareString(localname, ENDELEM_compSet))
  {
  parsingState= startComparisonConfiguration;
  }
else if(!XMLString::compareString(localname, ENDELEM_coco))
  {
  pCoco->compSets->push_back(pCoco->pCompSet);  // throw compSet into list
  parsingState= endComparisonConfiguration;
  }

XMLString::release(&strLocalname);
}



