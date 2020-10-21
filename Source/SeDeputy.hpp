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
    SePrompt* m_pSePrompt;
    
    Mutex*    m_pMutexBG;  // mutex for background thread
    Session*  m_pSession;  // rapidsse session
    Job*      m_pJob;      // job
  
    Timer     m_oTimer;      // the timer


    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    SeDeputy(SePrompt* pSePrompt);
   ~SeDeputy(void) noexcept(false);
  
  void vPromptBG(void);
  
    void vCreateSession(void);

    bool bExecutePipeline(void);
  
    Job* pJob(void);
    void vJob(Job* pJob);

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
