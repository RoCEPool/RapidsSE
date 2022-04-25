/*============================================================================*/
/* ApiHelper.cpp -- SE Api Helper                                             */
/*============================================================================*/

#include "BASE.h"

#include "ApiHelper.hpp"
#include "Api.hpp"

#include "Output.hpp"
#include "Job.hpp"

#include "TagConstants.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ApiHelper::ApiHelper(void)
{
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
/* delete job                                                                 */
/*----------------------------------------------------------------------------*/
void ApiHelper::vReapableJob(Job*& rpJob)
{
  Api::vReapableJob(rpJob);
}

/*----------------------------------------------------------------------------*/
/* execute job                                                                */
/*----------------------------------------------------------------------------*/
void ApiHelper::vExecuteJob(Job* pJob, TagSet* pCmdTagSet)
{
 Api::vExecuteJob(pJob,pCmdTagSet);
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
  eEC eError = Api::eParseCmd(pCmdTknVec,pCmdTagSet);
  if (EC_NONE != eError)
    return eError;

  //rpJob = Api::pAllocJob(m_pSession);
  Stz* szJobNm = pCmdTagSet->szValue(C_TAG_JOBNM);
  rpJob = Api::pAllocJob(szJobNm);
  
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
    pJob->pOutput()->vProcessBufferQueue(pJob);

  RASSERT(! pJob->pOutput()->m_bExecErr );
  RASSERT(! pJob->pOutput()->m_bCmdErr );

  if (pJob->bIsInError()) {
    vTrace("error: (node%lld) %s\n"_Stz, AM::NODE::iThisNodeId(),
           aEC[pJob->eJobError()]);
  }

}

/*----------------------------------------------------------------------------*/
/* reset results                                                              */
/*----------------------------------------------------------------------------*/
void ApiHelper::vResetResults(Job* pJob)
{
  pJob->pOutput()->m_pOutputQueue->vReset();
  
  pJob->pOutput()->vEndProcOneCmd();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
