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
void ApiHelper::vDeleteJob(void)
{
  Api::vReapableJob(m_pSession);
}

/*----------------------------------------------------------------------------*/
/* execute job                                                                */
/*----------------------------------------------------------------------------*/
void ApiHelper::vExecuteJob(void)
{
  Api::vExecuteJob(m_pSession);
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
void ApiHelper::vPrintResults(void)
{
  // start the buffer queue processing while the pipeline ops are running
  if (m_pSession->m_pSsnJob->bIsPipeline())
    m_pSession->m_pOutput->vProcessBufferQueue();

  RASSERT(! m_pSession->m_pOutput->m_bExecErr );
  RASSERT(! m_pSession->m_pOutput->m_bCmdErr );

  if (m_pSession->m_pSsnJob->bIsInError()) {
    vTrace("error: (node%lld) %s\n"_Stz, AM::NODE::iThisNodeId(),
           aEC[m_pSession->m_pSsnJob->eJobError()]);
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
