/*============================================================================*/
/* RapidsSE.hpp - rapids storage engine                                       */
/*============================================================================*/

#pragma once

#include "StorageEngine.h"

#define RAPIDSSE_VERSION_STRING "0.1.1"

namespace StorageEngine
{
  typedef NP<Task,CN_TASK> NP_Task;
  
  class RapidsSE
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    static void   vCheckAssumptions(void);
    
    static void   vDumpDummy(eCN eClass, Object* pObject, const char* pTab);
    static void   vDumpEnums(eCN eClass, Object* pObject, const char* pTab);
    static void   vDumpInvalid(eCN eClass, Object* pObject, const char* pTab);
    static void   vDumpUndefined(eCN eClass, Object* pObject, const char* pTab);

    static void   vExecute(void);
    static void   vPoolClose(void);
    static void   vPoolOpen(void);
    static void   vSEitfDelete(SEitf* pSEitf);
    static SEitf* pSEitfNew(void);

    //-------------------------------------------------------------------------
    
    static void vAttachObjects(void);
    static void vAttachRegion(void);
    static void vCreateObjects(void);
    static void vCreateRegion(void);
    static void vDicCompile(NP_Task npBootTask);
    static void vDropRegion(void);
    static void vHandler(Error* pError);
    static void vRunCommand(void);
    static void vRunDocol(void);
    static void vStartActors(void);
    static void vSystemInit(void);
    static void vSystemStatus(void);
    static void vSystemStop(void);
    static bool bTestRegion(void);
    static void vTlsInit(void);
    static void vWaitFlag(void);

    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/