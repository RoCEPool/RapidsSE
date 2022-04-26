/*============================================================================*/
/* ApiHelper.hpp -- SE Api Helper                                             */
/*============================================================================*/
#pragma once

namespace STORE
{
  class ApiHelper
  {
  protected:

  public:
    ApiHelper(void);
   ~ApiHelper(void) noexcept(false);


    i8   iCommandKind(void);
  
    void vExecuteJob(Job* pJob, TagSet* pCmdTagSet);

    void* pForkDemon(Stz* szPipeline);
  
    i8   iJobId(void);

    void vKillDemon(void*);

    void* pMetaData(void*);

    void vPipeoutResults(Op* pCurrOp, Operand* pOperand, Task* pTriggeringTask);
    eEC  ePrepareJob(Job*& rpJob, Stz* szCmd);
    eEC  ePrepareJob(Job*& rpJob, Stz* szCmd, TokenVec* pCmdTknVec, TagSet* pCmdTagSet);
    void vPrintResults(Job* pJob);
  
    void vReapableJob(Job*& rpJob);
    void vResetResults(Job* pJob);
    i8   nRowCount(void);
    i8   nRowsAffected(void);

    void vTerminateDemon(void*);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
