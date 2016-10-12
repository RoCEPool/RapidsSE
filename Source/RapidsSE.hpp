/*============================================================================*/
/* RapidsSE.hpp -- Rapids Storage Engine                                      */
/*============================================================================*/

#pragma once

#include "StorageEngine.h"

#define RAPIDSSE_VERSION_STRING "0.1.2"

namespace StorageEngine
{
  typedef NP<Task,CN_TASK> NP_Task;
  
  class RapidsSE
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    
    static void   vExecute(void);
    static void   vPoolClose(void);
    static void   vPoolOpen(void);
    static bool   bPoolOpen(s& sPoolName);
    static void   vSEitfDelete(SEitf* pSEitf);
    static SEitf* pSEitfNew(void);

    //-------------------------------------------------------------------------
    
    static void vAttachObjects(void);
    static void vCreateObjects(void);
    static void vDicCompile(NP_Task npBootTask);
    static void vHandler(Error* pError);
    static void vInitJumboHeap(void);
    static void vRunCommand(void);
    static void vRunDocol(void);
    static void vStartActors(void);
    static void vSystemInit(void);
    static void vSystemStatus(void);
    static void vSystemStop(void);
    static void vWaitFlag(void);

    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
