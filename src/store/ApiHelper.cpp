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
  _precondition_(_ADDRESS_, _MAGIC_);

  m_pSession  = nullptr;

  _postcondition_(_ADDRESS_, _MAGIC_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ApiHelper::~ApiHelper(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_);

}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allocate session                                                           */
/*----------------------------------------------------------------------------*/
void ApiHelper::vAllocateSession(void)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  m_pSession = Api::pAllocSession(SK_EXTERNAL);
}

/*----------------------------------------------------------------------------*/
/* delete job                                                                 */
/*----------------------------------------------------------------------------*/
void ApiHelper::vDeleteJob(void)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  Api::vDeleteJob(m_pSession);
}

/*----------------------------------------------------------------------------*/
/* execute job                                                                */
/*----------------------------------------------------------------------------*/
void ApiHelper::vExecuteJob(void)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  Api::vExecuteJob(m_pSession);
}

/*----------------------------------------------------------------------------*/
/* free session                                                               */
/*----------------------------------------------------------------------------*/
void ApiHelper::vFreeSession(void)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  Api::vFreeSession(m_pSession);
  m_pSession = nullptr;
}

/*----------------------------------------------------------------------------*/
/* pipeout the resuts                                                         */
/*----------------------------------------------------------------------------*/
void ApiHelper::vPipeoutResults(Op* pCurrOp,
                                Operand* pOperand,
                                Task* pTriggeringTask)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  // to be coded
  NOT_IMPLEMENTED_YET;
}

/*----------------------------------------------------------------------------*/
/* prepare job                                                                */
/*----------------------------------------------------------------------------*/
eEC ApiHelper::ePrepareJob(Stz* szCmd)
{
  _precondition_(_ADDRESS_, _MAGIC_);
  
  TokenVec* pCmdTknVec = new TokenVec;
  
  pCmdTknVec->vTokeniseCpp(szCmd);
  
  eEC eError = ApiHelper::ePrepareJob(pCmdTknVec);

  delete pCmdTknVec;
  
  return eError;
}

/*----------------------------------------------------------------------------*/
/* prepare job                                                                */
/*----------------------------------------------------------------------------*/
eEC ApiHelper::ePrepareJob(TokenVec* pCmdTknVec)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  eEC eError = Api::ePrepareJob(m_pSession,pCmdTknVec);
  
  return eError;
}

/*----------------------------------------------------------------------------*/
/* print results                                                              */
/*----------------------------------------------------------------------------*/
void ApiHelper::vPrintResults(void)
{
  _precondition_(_ADDRESS_, _MAGIC_);
  
  // start the buffer queue processing while the pipeline ops are running
  if (m_pSession->m_pSsnJob->bIsPipeline())
    m_pSession->m_pOutput->vProcessBufferQueue();

  RASSERT(! m_pSession->m_pOutput->m_bExecErr );
  RASSERT(! m_pSession->m_pOutput->m_bCmdErr );

  if (m_pSession->m_pSsnJob->bJobHasError()) {
    vTrace("error: (node%lld) %s\n"_Stz, AM::iThisNodeNo(),
           aEC[m_pSession->m_pSsnJob->eJobError()]);
  }

}

/*----------------------------------------------------------------------------*/
/* reset results                                                              */
/*----------------------------------------------------------------------------*/
void ApiHelper::vResetResults(void)
{
  _precondition_(_ADDRESS_, _MAGIC_);
  
  m_pSession->m_pOutput->m_pOutputQueue->vReset();
  
  m_pSession->m_pOutput->vEndProcOneCmd();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
