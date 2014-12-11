#include "defines.h"
#include "ComparatorInterface.h"
#include "InputParams.h"
#include "ComparatorError.h"

int main(int argc, char* argv[])
{
    ComparatorError *pError= new ComparatorError();
    InputParams *pInPa= new InputParams(pError);
    int error=0;

    pInPa->parseArgs(argc, argv);
    if(pInPa->pErr->error==0)
      {
      return 0;  // finish program after printing of help menu
      }
    else if(pInPa->pErr->error<0)
      {
      pError->printLogfile();
      return 0;
      }

    #ifdef DEBUG_INPUTPARAMS
    pInPa->printInputParams();
    system("PAUSE");
    #endif

    ComparatorInterface *pCI= new ComparatorInterface(pInPa, pError);

    pCI->startComparator();
    if(pCI->pErr->error==-1)
      {
      std::cout<<"Comparator was not able to finish task. "<<std::endl<<"Program terminated."<<std::endl;
      }
    else
      {
      #ifdef DEBUG_GENERAL
      std::cout<<"Thank you!";
      #endif
      }


   delete pCI; pCI= NULL;
   delete pInPa; pInPa= NULL;

   return 0;
};
