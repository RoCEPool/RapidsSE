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

    b8        m_bAvailable;
    b8        m_bContinue;
    b8        m_bTerminated;
    //b8        m_bSystemCloser; // is this deputy the system closer?

    eEC       m_eError;
  
    //s256      m_sFolderPath;   // folder path
    //JobId     m_tLastJobId;    // last job id

    s4K       m_sToDo;         // prompt to do

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    SeDeputy(i8 i_Deputy, SePrompt* pSePrompt);
   ~SeDeputy(void) noexcept(false);
  
    static void* aEntryPoint(void* pArg);
  
    void vDeputyLoop(void);

    void vDelete_Job(Job* pJob);
    eEC  eExecute_Job(TokenVec *pInpTknVec, TagSet *pInpTagSet);
    Job* pNew_Job(void);

    // -- -- -- -- -- --
  
    bool bAvailable(void);
    void vDeputiseJob(SePrompt* pSePrompt);
    eEC  eError(void);
    void vError(eEC eError);
    Input* pInput(void);
    void vMutexLock(Object* pLockBy);
    void vMutexWaitAndUnlock(Object* pLockBy);
    void vReset(void);
    void vTerminate(void);
    bool bTerminated(void);
    Stz* szToDo(void);
    void vToDo(Stz* szToDo);

  private:
    Footer* aFooter(void);
  };

  SeDeputy* _pSeDeputy(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
