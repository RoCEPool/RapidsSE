/*============================================================================*/
/* SeDeputy.hpp -- the se> prompt deputy                                      */
/*============================================================================*/
#pragma once

#include "Prompt.h"

namespace Prompt
{
  class SeDeputy : public Object
  {
  private:
    i8        m_i_Deputy;
    SePrompt* m_pSePrompt;
    
    Thread*   m_pThread;

    Session*  m_pSession;  // rapidsse session
    
    bool      m_bAvailable;
    bool      m_bContinue;
    bool      m_bExited;

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    SeDeputy(i8 i_Deputy, SePrompt* pSePrompt);
   ~SeDeputy(void) noexcept(false);
  
    static void* aEntryPoint(void* pArg);
  
    void vDeputyLoop(void);
    void vExecuteJob(void);
    void vDeleteJob(void);

    // -- -- -- -- -- --
  
    bool bAvailable(void);

    void vCreateSession(void);

    void vDeputiseJob(void);
  
    Job* pJob(void);
  
    bool bProcessPipelineResults(void);
  
    void vReportTime(void);

    Session* pSession(void);

    void vTerminate(void);
    bool bTerminated(void);

  private:

    Footer* aFooter(void);
  };

  SeDeputy* _pSeDeputy(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
