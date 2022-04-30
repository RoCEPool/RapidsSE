/*============================================================================*/
/* Steps_Api.cpp - Api step group                                             */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "eYX.h"

#include "Node.hpp"
#include "Node_I.hpp"
#include "Grid.hpp"
#include "Api.hpp"
#include "Op.hpp"
#include "OpSpec.hpp"
#include "Job.hpp"
#include "JobList.hpp"
#include "LogicUnit.hpp"

#include "Procedure.hpp"

#include "MY.hpp"
#include "TagConstants.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Api(void)
{
  vAddStep("api::addProcedure"_Stz,        &Step::vApiAddProcedure);
  vAddStep("api::derefMetricsDisable"_Stz, &Step::vApiDerefMetricsDisable);
  vAddStep("api::derefMetricsEnable"_Stz,  &Step::vApiDerefMetricsEnable);
  vAddStep("api::forkChild_noWait"_Stz,    &Step::vApiForkChild_noWait);
  vAddStep("api::forkChild_wait"_Stz,      &Step::vApiForkChild_wait);
  vAddStep("api::jobMetricsDisable"_Stz,   &Step::vApiJobMetricsDisable);
  vAddStep("api::jobMetricsEnable"_Stz,    &Step::vApiJobMetricsEnable);
  vAddStep("api::reapDisable"_Stz,         &Step::vApiReapDisable);
  vAddStep("api::reapEnable"_Stz,          &Step::vApiReapEnable);
  vAddStep("api::reapJobs"_Stz,            &Step::vApiReapJobs);
  vAddStep("api::sysMetricsDisable"_Stz,   &Step::vApiSysMetricsDisable);
  vAddStep("api::sysMetricsEnable"_Stz,    &Step::vApiSysMetricsEnable);
}

/*----------------------------------------------------------------------------*/
/* api::addProcedure -- add a procedure                                       */
/*----------------------------------------------------------------------------*/
void Step::vApiAddProcedure(void)
{
  ENTRY();
  
  s48K sProc;
  m_pDataStack->vPopStr(sProc,__func__);
  
  s48K sWithTagStr;
  m_pDataStack->vPopStr(sWithTagStr,__func__);
  
  if (AM::TRACE::bVerbose(C_VB_BOOT)) {
    vTrace("AddProcedure: %s\n"_Stz,sProc.szStz());
    vTrace("WithTags: %s\n"_Stz,sWithTagStr.szStz());
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  TokenVec* pDmnTknVec = TokenVec::pTokeniseSen(sProc);
  
  pDmnTknVec->vIntegrityCheck();
  
  Token* pTkn0 = pDmnTknVec->pGet(0);
  ASSERT(pTkn0->bSame("procedure"_Stz) || pTkn0->bSame("procedureLocal"_Stz));
  UNUSED(pTkn0);

  //Stz* ssProcNm = pDmnTknVec->pGet(1)->szChars();
  
  Procedure::vAddProcedure(pDmnTknVec);
}

/*----------------------------------------------------------------------------*/
/* api::derefMetricsDisable -- disable deref metrics                          */
/*----------------------------------------------------------------------------*/
void Step::vApiDerefMetricsDisable(void)
{
  ENTRY();
  
  AM::SYSARGS::vDerefMetrics(false);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* api::dferefMetricsEnable -- enable deref metrics                           */
/*----------------------------------------------------------------------------*/
void Step::vApiDerefMetricsEnable(void)
{
  ENTRY();
  
  AM::SYSARGS::vDerefMetrics(true);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* api::forkChild_noWait -- fork child job (no wait)                          */
/*----------------------------------------------------------------------------*/
void Step::vApiForkChild_noWait(void)
{
  ENTRY();
  
  s48K sCmd;
  m_pDataStack->vPopStr(sCmd,__func__);
   
  s48K sCmdTagStr;
  m_pDataStack->vPopStr(sCmdTagStr,__func__);

  if (AM::TRACE::bVerbose(C_VB_API)) {
    vTrace("SpawnDemon: %s\n"_Stz,sCmd.szStz());
    vTrace("  WithTags: %s\n"_Stz,sCmdTagStr.szStz());
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  TokenVec* pCmdTknVec = TokenVec::pTokeniseSen(sCmd);
    
  TagSet* pCmdTagSet = new TagSet;
  TokenVec* pTagTknVec = TokenVec::pTokeniseSen(sCmdTagStr);
  Api::vPopulateTagSet(pCmdTagSet,pTagTknVec);

  pCmdTagSet->vReplace(C_TAG_NOWAIT,1);

  JobId tChildJobId;
  
  Job* pJob = nullptr;
  eEC eError = Api::eForkJob(tChildJobId,m_pJob,
                           pCmdTknVec,pCmdTagSet,
                           pJob // TO BE REMOVED
                           );
  RASSERT(EC_NONE == eError);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pDataStack->vPushInt(true);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* api::forkChild_wait -- fork child job (wait)                               */
/*----------------------------------------------------------------------------*/
void Step::vApiForkChild_wait(void)
{
  ENTRY();

  // pop command and tag string
  
  s48K sCmd;
  m_pDataStack->vPopStr(sCmd,__func__);

  s48K sCmdTagStr;
  m_pDataStack->vPopStr(sCmdTagStr,__func__);

  if (AM::TRACE::bVerbose(C_VB_API)) {
    vTrace("ExecuteCommand: %s\n"_Stz,sCmd.szStz());
    vTrace("      WithTags: %s\n"_Stz,sCmdTagStr.szStz());
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  
  TokenVec* pCmdTknVec = TokenVec::pTokeniseSen(sCmd);
    
  TagSet* pCmdTagSet = new TagSet;
  TokenVec* pTagTknVec = TokenVec::pTokeniseSen(sCmdTagStr);
  Api::vPopulateTagSet(pCmdTagSet,pTagTknVec);

  pCmdTagSet->vReplace(C_TAG_WAIT,1);
  
  JobId tChildJobId;
  
  Job* pJob = nullptr;
  eEC eError = Api::eForkJob(tChildJobId,m_pJob,
                           pCmdTknVec,pCmdTagSet,
                           pJob // TO BE REMOVED
                           );
  RASSERT(EC_NONE == eError);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_pDataStack->vPushInt(true);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* api::jobMetricsDisable -- disable job metrics                              */
/*----------------------------------------------------------------------------*/
void Step::vApiJobMetricsDisable(void)
{
  ENTRY();
  
  AM::SYSARGS::vJobMetrics(false);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* api::jobMetricsEnable -- enable job metrics                                */
/*----------------------------------------------------------------------------*/
void Step::vApiJobMetricsEnable(void)
{
  ENTRY();
  
  AM::SYSARGS::vJobMetrics(true);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* api::reapDisable -- disble reaping of retired jobs                         */
/*----------------------------------------------------------------------------*/
void Step::vApiReapDisable(void)
{
  ENTRY();
  
  AM::SYSTEM::vReapEnabled(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* api::reapEnable -- enable reaping of retired jobs                          */
/*----------------------------------------------------------------------------*/
void Step::vApiReapEnable(void)
{
  ENTRY();
  
  AM::SYSTEM::vReapEnabled(true);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* api::reapJobs -- reap retired jobs                                         */
/*----------------------------------------------------------------------------*/
void Step::vApiReapJobs(void)
{
  ENTRY();
  
  i8 iForce = (i8)m_pDataStack->iPopBol(__func__);

  Api::vReapJobsOnce(iForce);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* api::sysMetricsDisable -- disable sys metrics                              */
/*----------------------------------------------------------------------------*/
void Step::vApiSysMetricsDisable(void)
{
  ENTRY();
  
  AM::SYSARGS::vSysMetrics(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* api::sysMetricsEnable -- enable sys metrics                                */
/*----------------------------------------------------------------------------*/
void Step::vApiSysMetricsEnable(void)
{
  ENTRY();
  
  AM::SYSARGS::vSysMetrics(true);

  EXIT();
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
