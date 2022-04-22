/*============================================================================*/
/* SeDeputy.hpp -- the se> prompt deputy                                      */
/*============================================================================*/
#pragma once

#include "BASE.h"
#include "ApiHelper.hpp"


namespace PROMPT
{
  class SeDeputy : public Object
                 , public ApiHelper
  {
  private:
    i8        m_i_Deputy;
    SePrompt* m_pSePrompt;
    
    Thread*   m_pThread;

    Input*    m_pInput;        // input stream

    bool      m_bAvailable;
    bool      m_bContinue;
    bool      m_bTerminated;

    Job*      m_pJob;
  
    s256      m_sFolderPath;   // folder path
    JobId     m_tLastJobId;    // last job id

    TagSet    m_oCmdTagSet;    // extra tags gleaned from the command

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
    void vRepeableJob(void);

    eEC  ePrepareJob(TokenVec* pCmdTknVec);

    // -- -- -- -- -- --
  
    bool bAvailable(void);
    TagSet* pCmdTagSet(void);
    void vDeputiseJob(void);
    Input* pInput(void);
    Job* pJob(void);
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
