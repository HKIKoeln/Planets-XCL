#include "defines.h"
#include "ComparatorInterface.h"
#include "InputParams.h"
#include "ComparatorError.h"

int main(int argc, char* argv[])
{
    InputParams *pIP= new InputParams();
    int error=0;

    error= pIP->parseArgs(argc, argv);
    if(error<0)
      {
      std::cout<<"Error while parsing input arguments. Check input parameters.";
      pIP->cleanUp();
      delete pIP;
      return 0;
      }

    #ifdef DEBUG_INPUTPARAMS
    pIP->printInputParams();
    system("PAUSE");
    #endif

    ComparatorInterface *pCI= new ComparatorInterface(pIP);

    pCI->startComparator();

    if(pCI->pErr->error==-1)
      {
      #ifdef DEBUG_GENERAL
      std::cout<<"Comparison couldn't be executed correctly. See logfile for details."<<std::endl;
      system("PAUSE");
      #endif
      }
    else
      {
      //std::cout<<"DONE...";
      }

   // delete pCI; pCI= NULL;
   // delete pIP; pIP= NULL;

    return 0;
};
