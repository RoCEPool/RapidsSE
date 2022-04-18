/*============================================================================*/
/* ApiHelper.cpp -- SE Api Helper                                             */
/*============================================================================*/

#include "BASE.h"

#include "ApiHelper.hpp"
#include "Api.hpp"

#include "Output.hpp"
#include "Session.hpp"
#include "Job.hpp"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ApiHelper::ApiHelper(void)
{
  m_pSession  = nullptr;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ApiHelper::~ApiHelper(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allocate session                                                           */
/*----------------------------------------------------------------------------*/
void ApiHelper::vAllocateSession(void)
{
  m_pSession = Api::pAllocSession(SK_EXTERNAL);
}

/*----------------------------------------------------------------------------*/
/* delete job                                                                 */
/*----------------------------------------------------------------------------*/
void ApiHelper::vReapableJob(Job*& rpJob)
{
  Api::vReapableJob(m_pSession,rpJob);
}

/*----------------------------------------------------------------------------*/
/* execute job                                                                */
/*----------------------------------------------------------------------------*/
void ApiHelper::vExecuteJob(Job* pJob)
{
  Api::vExecuteJob(m_pSession,pJob);
}

/*----------------------------------------------------------------------------*/
/* fork demon                                                                 */
/*----------------------------------------------------------------------------*/
void* ApiHelper::pForkDemon(Stz* szPipeline)
{
  // to be coded
  NOT_IMPLEMENTED_YET;
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* free session                                                               */
/*----------------------------------------------------------------------------*/
void ApiHelper::vFreeSession(void)
{
  Api::vFreeSession(m_pSession);
  m_pSession = nullptr;
}

/*----------------------------------------------------------------------------*/
/* kill demon                                                                 */
/*----------------------------------------------------------------------------*/
void ApiHelper::vKillDemon(void*)
{
  // to be coded
  NOT_IMPLEMENTED_YET;
}

/*----------------------------------------------------------------------------*/
/* pipeout the results                                                        */
/*----------------------------------------------------------------------------*/
void ApiHelper::vPipeoutResults(Op* pCurrOp,
                                Operand* pOperand,
                                Task* pTriggeringTask)
{
  // to be coded
  NOT_IMPLEMENTED_YET;
}

/*----------------------------------------------------------------------------*/
/* prepare job                                                                */
/*----------------------------------------------------------------------------*/
eEC ApiHelper::ePrepareJob(Stz* szCmd, Job*& rpJob)
{
  TokenVec* pCmdTknVec = new TokenVec;
  
  pCmdTknVec->vTokeniseCpp(szCmd);
  
  eEC eError = ApiHelper::ePrepareJob(pCmdTknVec,rpJob);

  delete pCmdTknVec;
  
  return eError;
}

/*----------------------------------------------------------------------------*/
/* prepare job                                                                */
/*----------------------------------------------------------------------------*/
eEC ApiHelper::ePrepareJob(TokenVec* pCmdTknVec, Job*& rpJob)
{
  eEC eError = Api::ePrepareJob(m_pSession,pCmdTknVec,rpJob);
  
  return eError;
}

/*----------------------------------------------------------------------------*/
/* print results                                                              */
/*----------------------------------------------------------------------------*/
void ApiHelper::vPrintResults(Job* pJob)
{
  // start the buffer queue processing while the pipeline ops are running
  if (pJob->bIsPipeline())
    m_pSession->m_pOutput->vProcessBufferQueue(pJob);

  RASSERT(! m_pSession->m_pOutput->m_bExecErr );
  RASSERT(! m_pSession->m_pOutput->m_bCmdErr );

  if (pJob->bIsInError()) {
    vTrace("error: (node%lld) %s\n"_Stz, AM::NODE::iThisNodeId(),
           aEC[pJob->eJobError()]);
  }

}

/*----------------------------------------------------------------------------*/
/* reset results                                                              */
/*----------------------------------------------------------------------------*/
void ApiHelper::vResetResults(void)
{
  m_pSession->m_pOutput->m_pOutputQueue->vReset();
  
  m_pSession->m_pOutput->vEndProcOneCmd();
}

/*----------------------------------------------------------------------------*/
/* get response time (in milliseconds)                                        */
/*----------------------------------------------------------------------------*/
i8 ApiHelper::iResponseTimeMsec(void)
{
  i8 iTimeMsec = m_pSession->m_pSsnJob->iResponseTimerMsec();
  
  return iTimeMsec;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
