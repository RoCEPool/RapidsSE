/*============================================================================*/
/* ApiHelper.hpp -- SE Api Helper                                             */
/*============================================================================*/
#pragma once

namespace STORE
{
  class ApiHelper
  {
  protected:
    Session* m_pSession;  // rapidsse session

  public:
    ApiHelper(void);
   ~ApiHelper(void) noexcept(false);

    void vAllocateSession(void);

    i8   iCommandKind(void);
  
    void vDeleteJob(void);
  
    void vExecuteJob(void);

    void* pForkDemon(Stz* szPipeline);
    void vFreeSession(void);
  
    i8   iJobId(void);

    void vKillDemon(void*);

    void* pMetaData(void*);

    void vPipeoutResults(Op* pCurrOp, Operand* pOperand, Task* pTriggeringTask);
    eEC  ePrepareJob(Stz* szCmd, Job*& rpJob);
    eEC  ePrepareJob(TokenVec* pCmdTknVec, Job*& rpJob);
    void vPrintResults(void);
  
    void vResetResults(void);
    i8   iResponseTimeMsec(void);
    i8   nRowCount(void);
    i8   nRowsAffected(void);

    void vTerminateDemon(void*);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
