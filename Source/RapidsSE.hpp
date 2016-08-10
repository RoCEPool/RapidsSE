/*============================================================================*/
/* RapidsSE.hpp - rapids storage engine                                       */
/*============================================================================*/

#pragma once

#include "StorageEngine.h"

namespace StorageEngine
{
  typedef NP<Task,CN_TASK> NP_Task;

  class RapidsSE
  {
  private:

  public:
    static void vCheckAssumptions(void);
    
    static void vExecute(void);
    static void vPoolClose(void);
    static void vPoolOpen(void);

    static void vAttachRegionNew(void);
    static void vCreateRegionNew(void);
    static void vDropRegionNew(void);
    
    //-------------------------------------------------------------------------
    
    static void vAttachObjects(void);
    static void vAttachRegion(void);
    static void vCreateObjects(void);
    static void vCreateRegion(void);
    static void vDicInit(NP_Task npBootTask);
    static void vHandler(Error* pError);
    static void vRunCommand(void);
    static void vRunDocol(void);
    static void vSeitfPrompt(void);
    static void vSystemInit(void);
    static void vSystemStart(void);
    static void vSystemStop(void);
    static void vTlsInit(void);
    static void vWaitFlag(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/