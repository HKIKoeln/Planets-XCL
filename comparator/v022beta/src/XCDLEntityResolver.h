#include <xercesc/sax/EntityResolver.hpp>
#include <xercesc/sax/InputSource.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>
#include <xercesc/util/XMLString.hpp>
#include <string>


/******************************************************************************
Name: XCDLEntityResolver.h, PCREntityResolver.h

Description:
This is a customized entity resolver for XCDL/ PCR documents. It compares
the systemId for entities to be resolved with a given one. The latter
changes according to the entityResolverState.

Example for XCDL document:
The first entity xerces resolves is xsi:schemaLocation.
The systemId is the parameter that follows the URL:
<xsi:schemaLocation="http://www.planets.project.eu/xcl/  ../res/xcl/xcdl/XCDLCore.xsd">

The method resolveEntity() compares this systemId with the given one; for
entityResolverState==0, this is the one indicated within the method for this
is the first entity within a XCDL document to be resolved; the entityResolverState is then
incremented to expect the import and include schemas. These are resolved according to
the location in XCDLCore.xsd, so in this case NULL is passed back, indicating to not
change the InputSource for Xerces.
******************************************************************************/

class PCREntityResolver : public EntityResolver
{
public:
    PCREntityResolver() {setEntityResolverState(0);}
    InputSource* resolveEntity (const XMLCh* const publicId, const XMLCh* const systemId);
    inline int getEntityResolveState() {return entityResolverState;}
    inline void setEntityResolverState(int newState) {entityResolverState= newState;}

    int entityResolverState;  // var for PCREntityResolver state;
};


class XCDLEntityResolver : public EntityResolver
{
public:
    XCDLEntityResolver() {setEntityResolverState(0);}
    InputSource* resolveEntity (const XMLCh* const publicId, const XMLCh* const systemId);
    inline int getEntityResolveState() {return entityResolverState;}
    inline void setEntityResolverState(int newState) {entityResolverState= newState;}
private:
    int entityResolverState;  // var for XCDLEntityResolver state;
};


InputSource* PCREntityResolver :: resolveEntity (const XMLCh* const publicId, const XMLCh* const systemId)
{
   //std::cout<<"SystemID: "<<std::string(XMLString::transcode(systemId))<<std::endl;

   if(getEntityResolveState()==0)
     {
     #ifdef __OS_DEVELOP
     const XMLCh* resString= XMLString::transcode("c:\\comparator\\res\\schemas\\pcr\\pcr.xsd");
     #endif
     #ifdef __OS_WIN
     const XMLCh* resString= XMLString::transcode("res\\schemas\\pcr\\pcr.xsd");
     #endif
     #ifdef __OS_OSMAC
     const XMLCh* resString= XMLString::transcode("res/schemas/pcr/pcr.xsd");
     #endif
     #ifdef __OS_OTHERS
     const XMLCh* resString= XMLString::transcode("/opt/comparator/res/schemas/pcr/pcr.xsd");
     #endif
     setEntityResolverState(1);

     // if and only if resString is the same as systemId, load file from that location,
     // else use the new file located in resString:
     if(!XMLString::compareString(systemId, resString))
       {
       return NULL;
       }
     else
       {
       #ifdef DEBUG_XMLVALIDATION
       std::cout<<"Changed xsd file location!";
       std::cout<<"New xsd location: "<<std::string(XMLString::transcode(resString))<<std::endl;
       #endif
       // Return local schema:
       InputSource *pIP= new LocalFileInputSource(resString);
       pIP->setIssueFatalErrorIfNotFound(true);
       //if(pIP->getIssueFatalErrorIfNotFound()==true) std::cout<<"Issue for fatal error sret";
       return pIP; //return new LocalFileInputSource(resString);
       }
     }
   //  resolving <import> and <include> tags: don't search in new path, search at given locations
   else
     {
     return NULL;
     //return new LocalFileInputSource(systemId);
     }
}

InputSource* XCDLEntityResolver :: resolveEntity (const XMLCh* const publicId, const XMLCh* const systemId)
{
   //std::cout<<"SystemID: "<<std::string(XMLString::transcode(systemId))<<std::endl;

   if(getEntityResolveState()==0)
     {
     #ifdef __OS_DEVELOP
     const XMLCh* resString= XMLString::transcode("c:\\comparator\\res\\schemas\\xcdl\\XCDLCore.xsd");
     #endif
     #ifdef __OS_WIN
     const XMLCh* resString= XMLString::transcode("res\\schemas\\xcdl\\XCDLCore.xsd");
     #endif
     #ifdef __OS_OSMAC
     const XMLCh* resString= XMLString::transcode("res/schemas/xcdl/XCDLCore.xsd");
     #endif
     #ifdef __OS_OTHERS
     const XMLCh* resString= XMLString::transcode("/opt/comparator/res/schemas/xcdl/XCDLCore.xsd");
     #endif
     setEntityResolverState(1);

     // if and only if resString is the same as systemId, load file from that location,
     // else use the new file located in resString:
     if(!XMLString::compareString(systemId, resString))
       {
       return NULL;
       }
     else
       {
       #ifdef DEBUG_XMLVALIDATION
       std::cout<<"Changed xsd file!";
       std::cout<<"New xsd location: "<<std::string(XMLString::transcode(resString))<<std::endl;
       #endif
       // Return local schema:
       InputSource *pIP= new LocalFileInputSource(resString);
       pIP->setIssueFatalErrorIfNotFound(true);
       //if(pIP->getIssueFatalErrorIfNotFound()==false) std::cout<<"Issue for fatal error sret";
       return pIP; //return new LocalFileInputSource(resString);
       }
     }
   //  resolving <import> and <include> tags: don't search in new path, search at given locations
   else
     {
     return NULL;
     }
}
