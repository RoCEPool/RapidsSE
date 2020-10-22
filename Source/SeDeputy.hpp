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
    Job*      m_pJob;      // job
  
    Timer     m_oTimer;    // the timer
  
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

    bool bAvailable(void);
    void vAvailable(bool bAvailable);

    bool bContinue(void);
    void vContinue(bool bContinue);

    bool bExited(void);
    void vExited(bool bExited);

    void vDeputyLoop(void);

    void vDeleteJob(void);
  
    void vExecuteJob(void);
  
    void vCreateSession(void);

    bool bExecutePipeline(void);
  
    Job* pJob(void);
    void vJob(Job* pJob);

    Mutex* pMutex(void);
  
    void vReportTime(void);

    Session* pSession(void);
    void     vSession(Session* pSession);

  private:

    Footer* aFooter(void);
  };

  SeDeputy* _pSeDeputy(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
