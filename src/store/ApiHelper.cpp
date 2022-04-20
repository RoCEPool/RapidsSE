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
  Api::vReapableJob(rpJob);
}

/*----------------------------------------------------------------------------*/
/* execute job                                                                */
/*----------------------------------------------------------------------------*/
void ApiHelper::vExecuteJob(Job* pJob)
{
  Api::vExecuteJob(pJob);
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
eEC ApiHelper::ePrepareJob(Job*& rpJob, Stz* szCmd)
{
  TokenVec* pCmdTknVec = new TokenVec;
  
  pCmdTknVec->vTokeniseCpp(szCmd);
  
  TagSet* pCmdTagSet = new TagSet;
  
  eEC eError = ApiHelper::ePrepareJob(rpJob,pCmdTknVec,pCmdTagSet);

  delete pCmdTknVec;
  delete pCmdTagSet;
  return eError;
}

/*----------------------------------------------------------------------------*/
/* prepare job                                                                */
/*----------------------------------------------------------------------------*/
eEC ApiHelper::ePrepareJob(Job*& rpJob, TokenVec* pCmdTknVec, TagSet* pCmdTagSet)
{
  //TagSet* pCmdTagSet = & m_pSession->m_oCmdTagSet;

  eEC eError = Api::eParseCmd(pCmdTknVec,pCmdTagSet);
  if (EC_NONE != eError)
    return eError;

  rpJob = Api::pAllocJob(m_pSession);  

  eError = Api::ePrepareJob(rpJob,pCmdTknVec,pCmdTagSet);
  
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
