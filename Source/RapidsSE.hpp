/*============================================================================*/
/* RapidsSE.hpp -- Rapids Storage Engine                                      */
/*============================================================================*/
#pragma once

#define RAPIDSSE_VERSION_STRING "0.3.1"

namespace StorageEngine
{
  typedef NP<Task,CN_TASK> NP_Task;
  
  class RapidsSE
  {
  
  private:

    // must be last
    Footer m_xFooter;

  public:
    
    static void   vExecuteCmdLine(void);
    static void   vCheckPort(void);
    static void   vPoolClose(void);
    static void   vPoolOpen(void);
    static bool   bPoolOpen(StrVal& sPoolName);
    static void   vSEitfDelete(SEitf* pSEitf);

    //-------------------------------------------------------------------------
    
    static void vAttachObjects(void);
    static void vCreateObjects(void);
    static void vDicCompile(NP_Task npBootTask);
    static void vErrorHandler(Error* pError);
    static void vInitJumboHeap(void);
    static void vRunCommand(void);
    static void vRunDocol(void);
    static void vSignalHandler(int iSignal);
    static void vStartActors(void);
    static void vSystemInit(void);
    static void vSystemStatus(void);
    static void vSystemStop(void);
    static void vWaitFlag(void);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
