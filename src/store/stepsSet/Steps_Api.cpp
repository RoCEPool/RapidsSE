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
#include "Session.hpp"
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
  vAddStep("api::executeCommand"_Stz,      &Step::vApiExecuteCommand);
  vAddStep("api::forkCommand"_Stz,         &Step::vApiForkCommand);
  vAddStep("api::jobMetricsDisable"_Stz,   &Step::vApiJobMetricsDisable);
  vAddStep("api::jobMetricsEnable"_Stz,    &Step::vApiJobMetricsEnable);
  vAddStep("api::reapDisable"_Stz,         &Step::vApiReapDisable);
  vAddStep("api::reapEnable"_Stz,          &Step::vApiReapEnable);
  vAddStep("api::reapJobs"_Stz,            &Step::vApiReapJobs);
  vAddStep("api::spawnDemon"_Stz,          &Step::vApiSpawnDemon);
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
/* api::executeCommand -- execute a command (internally)                      */
/*----------------------------------------------------------------------------*/
void Step::vApiExecuteCommand(void)
{
  ENTRY();

  // pop command and tag string
  
  s48K sCmd;
  m_pDataStack->vPopStr(sCmd,__func__);

  s48K sWithTagStr;
  m_pDataStack->vPopStr(sWithTagStr,__func__);

  if (AM::TRACE::bVerbose(C_VB_API)) {
    vTrace("ExecuteCommand: %s\n"_Stz,sCmd.szStz());
    vTrace("      WithTags: %s\n"_Stz,sWithTagStr.szStz());
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Stz* szTagPath = nullptr;
  if (m_pJob)
    szTagPath = m_pJob->pJobTags()->szValue(C_TAG_PATH);
  
  
  
  Session* pSession = Api::pAllocSession(SK_INTERNAL);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tokenise the request
  
  TokenVec* pCmdTknVec = TokenVec::pTokeniseSen(sCmd);

  TagSet* pCmdTagSet =  new TagSet;

  eEC eError = Api::eParseCmd(pCmdTknVec,pCmdTagSet);
  if (EC_NONE != eError) {
    delete pCmdTknVec;
    delete pCmdTagSet;
    Api::vFreeSession(pSession);
    m_pDataStack->vPushInt(eError);
    m_pDataStack->vPushInt(false);
    EXIT();
    return;
   }
  
  //Job* pJob = Api::pAllocJob(pSession);
  Stz* szJobNm = pCmdTagSet->szValue(C_TAG_JOBNM);
  Job* pJob = Api::pAllocJob(szJobNm);

  TagSet* pJobTagSet = pJob->pJobTags();
  
  pJobTagSet->vReplace(C_TAG_PATH,C_SE_PATH);
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - -`- - - - - - - - - -
  
  
  eError = Api::ePrepareJob(pJob,pCmdTknVec,pCmdTagSet);

  delete pCmdTknVec;
  delete pCmdTagSet;

  if (eError) {
    
    Api::vFreeSession(pSession);
    m_pDataStack->vPushInt(eError);
    m_pDataStack->vPushInt(false);
    EXIT();
    return;
  }

  pJob->vStmtListCreate();
  Api::vExecuteJob(pJob,pCmdTagSet);
  
  m_pDataStack->vPushInt(true);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* api::forkCommand -- fork a command (internally)                            */
/*----------------------------------------------------------------------------*/
void Step::vApiForkCommand(void)
{
  ENTRY();

  Op* pResponseOp = _pOp(m_pDataStack->pPopPtr(__func__));

  // pop command and tag string
  
  s48K sCmd;
  m_pDataStack->vPopStr(sCmd,__func__);

  s48K sWithTagStr;
  m_pDataStack->vPopStr(sWithTagStr,__func__);

  if (AM::TRACE::bVerbose(C_VB_API)) {
    vTrace("ExecuteCommand: %s\n"_Stz,sCmd.szStz());
    vTrace("      WithTags: %s\n"_Stz,sWithTagStr.szStz());
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Stz* szTagPath = nullptr;
  if (m_pJob)
    szTagPath = m_pJob->pJobTags()->szValue(C_TAG_PATH);
 
  JobId tForkedJobId;
  eEC eError = Api::eForkCommand(sCmd,sWithTagStr,szTagPath,
                                 pResponseOp,tForkedJobId);
  
  if (EC_NONE == eError) {
    m_pDataStack->vPushInt(tForkedJobId.iAsI8());
    m_pDataStack->vPushInt(true);
  }
  else {
    m_pDataStack->vPushInt(eError);
    m_pDataStack->vPushInt(false);
  }
  
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
/* api::spawnDemon -- spawn a demon                                           */
/*----------------------------------------------------------------------------*/
void Step::vApiSpawnDemon(void)
{
  ENTRY();
  
  s48K sCmd;
  m_pDataStack->vPopStr(sCmd,__func__);
   
  s48K sCmdTagStr;
  m_pDataStack->vPopStr(sCmdTagStr,__func__);

  if (AM::TRACE::bVerbose(C_VB_API)) {
    vTrace("SpawnDemon: %s\n"_Stz,sCmd.szStz());
    vTrace("WithTags: %s\n"_Stz,sCmdTagStr.szStz());
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  TokenVec* pCmdTknVec = TokenVec::pTokeniseSen(sCmd);
  
  TagSet* pCmdTagSet = new TagSet;
  
  TokenVec* pTagStrTknVec = TokenVec::pTokeniseSen(sCmdTagStr);
  Api::vPopulateTagSet(pCmdTagSet,pTagStrTknVec);
  delete pTagStrTknVec;

  eEC eError = Api::eParseCmd(pCmdTknVec,pCmdTagSet);
  if (EC_NONE != eError)
    NEVER_REACH;
  
  Stz* szJobNm = pCmdTagSet->szValue(C_TAG_JOBNM);

  Job* pJob = Api::pAllocJob(szJobNm);
  TagSet* pJobTagSet = pJob->pJobTags();
  //pJobTagSet->vDumpSet();
    
  pJobTagSet->vReplace(C_TAG_PATH,C_SE_PATH);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - -` - - - - - - - - - -

  eError = Api::ePrepareJob(pJob,pCmdTknVec,pCmdTagSet);
  delete pCmdTknVec;
  delete pCmdTagSet;

  if (eError) {
    m_pDataStack->vPushInt(eError);
    m_pDataStack->vPushInt(false);
    EXIT();
  }
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (AM::TRACE::bVerbose(C_VB_API))
    pJobTagSet->vDumpSet();
  
  Api::vForkDemon(pJob);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pDataStack->vPushInt(true);
  
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
