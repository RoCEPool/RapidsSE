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
#include "SysArgs.hpp"
#include "AttachMap.hpp"

#include "MY.hpp"
#include "TagConstants.h"
#include "Jobs.hpp"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Api(void)
{
  vAddStep("api::forkChild"_Stz,       &Step::vApiForkChild);
  vAddStep("api::forkChildOld"_Stz,    &Step::vApiForkChildOld);
  vAddStep("api::forkChild2PC"_Stz,    &Step::vApiForkChild2PC);
  vAddStep("api::reapJobs"_Stz,        &Step::vApiReapJobs);
  vAddStep("api::spawnJob"_Stz,        &Step::vApiSpawnJob);
}

/*----------------------------------------------------------------------------*/
/* api::forkChild --                                                          */
/*----------------------------------------------------------------------------*/
void Step::vApiForkChild(void)
{
  ENTRY();
  
  s48K sCmd;
  m_pDataStack->vPopStr(sCmd,__func__);
   
  s48K sCmdTagStr;
  m_pDataStack->vPopStr(sCmdTagStr,__func__);

  if (AM::TRACE::bVerbose(C_VB_API)) {
    vTrace("ForkChild: %s\n"_Stz,sCmd.szStz());
    vTrace("  CmdTags: %s\n"_Stz,sCmdTagStr.szStz());
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  TokenVec* pCmdTknVec = TokenVec::pTokeniseSen(sCmd);
  TokenVec* pTagTknVec = TokenVec::pTokeniseSen(sCmdTagStr);
  TagSet*   pCmdTagSet = new TagSet;
  CmdHelper::vPopulateTagSet(pCmdTagSet,pTagTknVec);

  Stz* szJobNm = pCmdTknVec->pGet(0)->szChars();
  pCmdTagSet->vReplace(C_TAG_JOBNM,szJobNm);

  if (! pCmdTagSet->bHasName(C_TAG_PATH)) {
    if (m_pJob)
      pCmdTagSet->vReplace(C_TAG_PATH,m_pJob->pJobTags()->szValue(C_TAG_PATH));
    else
      pCmdTagSet->vReplace(C_TAG_PATH,C_SE_PATH_ROOT);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  JobId tParentId;
  JobId tOriginId;
  JobId tDistId;
  Job* pOuterJob = pOuterJobOfTask();
  tParentId = pOuterJob->tJobId();
  tOriginId = pOuterJob->tOriginId();
  tDistId   = pOuterJob->tDistId();
  
  JobId tChildJobId;
  Job* pChildJob_tbr = nullptr;
  eEC eError = Api::eForkJob(tChildJobId,
                             pCmdTknVec,pCmdTagSet,
                             tParentId,tOriginId,tDistId,
                             pOuterJob,    // TO BE REMOVED
                             pChildJob_tbr // TO BE REMOVED
                            );
  RASSERT(EC_NONE == eError);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  delete pCmdTagSet;
  delete pCmdTknVec;
  delete pTagTknVec;

  m_pDataStack->vPushInt(true);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* api::forkChildOld -- old (to go)                                              */
/*----------------------------------------------------------------------------*/
void Step::vApiForkChildOld(void)
{
  ENTRY();

  s48K sCmd;
  m_pDataStack->vPopStr(sCmd,__func__);

  s48K sCmdTagStr;
  m_pDataStack->vPopStr(sCmdTagStr,__func__);

  if (AM::TRACE::bVerbose(C_VB_API)) {
    vTrace("ForkChild: %s\n"_Stz,sCmd.szStz());
    vTrace("  CmdTags: %s\n"_Stz,sCmdTagStr.szStz());
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  TokenVec* pCmdTknVec = TokenVec::pTokeniseSen(sCmd);
  TokenVec* pTagTknVec = TokenVec::pTokeniseSen(sCmdTagStr);
  TagSet*   pCmdTagSet = new TagSet;
  CmdHelper::vPopulateTagSet(pCmdTagSet,pTagTknVec);

  Stz* szJobNm = pCmdTknVec->pGet(0)->szChars();
  pCmdTagSet->vReplace(C_TAG_JOBNM,szJobNm);

  if (! pCmdTagSet->bHasName(C_TAG_PATH)) {
    if (m_pJob)
      pCmdTagSet->vReplace(C_TAG_PATH,m_pJob->pJobTags()->szValue(C_TAG_PATH));
    else
      pCmdTagSet->vReplace(C_TAG_PATH,C_SE_PATH_ROOT);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  JobId tParentId;
  JobId tOriginId;
  JobId tDistId;
  Job* pOuterJob = pOuterJobOfTask();
  tParentId = pOuterJob->tJobId();
  tOriginId = pOuterJob->tOriginId();
  tDistId   = pOuterJob->tDistId();

  JobId tChildJobId;
  Job* pChildJob_tbr = nullptr;
  eEC eError = Api::eForkJob(tChildJobId,
                             pCmdTknVec,pCmdTagSet,
                             tParentId,tOriginId,tDistId,
                             pOuterJob,    // TO BE REMOVED
                             pChildJob_tbr // TO BE REMOVED
  );
  RASSERT(EC_NONE == eError);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  delete pCmdTagSet;
  delete pCmdTknVec;
  delete pTagTknVec;

  m_pDataStack->vPushInt(true);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* api::forkChild2Phase -- fork child job Api used by 2-phase commands        */
/*----------------------------------------------------------------------------*/
void Step::vApiForkChild2PC(void)
{
  ENTRY();

  s48K sCmd;
  m_pDataStack->vPopStr(sCmd,__func__);

  s48K sCmdTagStr;
  m_pDataStack->vPopStr(sCmdTagStr,__func__);

  if (AM::TRACE::bVerbose(C_VB_API)) {
    vTrace("ForkChild: %s\n"_Stz,sCmd.szStz());
    vTrace("  CmdTags: %s\n"_Stz,sCmdTagStr.szStz());
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  TokenVec* pCmdTknVec = TokenVec::pTokeniseSen(sCmd);
  TokenVec* pTagTknVec = TokenVec::pTokeniseSen(sCmdTagStr);
  TagSet*   pCmdTagSet = new TagSet;
  CmdHelper::vPopulateTagSet(pCmdTagSet,pTagTknVec);

  Stz* szJobNm = pCmdTknVec->pGet(0)->szChars();
  pCmdTagSet->vReplace(C_TAG_JOBNM,szJobNm);

  if (! pCmdTagSet->bHasName(C_TAG_PATH)) {
    if (m_pJob)
      pCmdTagSet->vReplace(C_TAG_PATH,m_pJob->pJobTags()->szValue(C_TAG_PATH));
    else
      pCmdTagSet->vReplace(C_TAG_PATH,C_SE_PATH_ROOT);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  JobId tParentId;
  JobId tOriginId;
  JobId tDistId;
  Job* pOuterJob = pOuterJobOfTask();
  tParentId = pOuterJob->tJobId();
  tOriginId = pOuterJob->tOriginId();
  tDistId   = pOuterJob->tDistId();

  JobId tChildJobId;
  Job* pChildJob_tbr = nullptr;
  eEC eError = Api::eForkJob(tChildJobId,
                             pCmdTknVec,pCmdTagSet,
                             tParentId,tOriginId,tDistId,
                             pOuterJob,    // TO BE REMOVED
                             pChildJob_tbr // TO BE REMOVED
  );
  RASSERT(EC_NONE == eError);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  delete pCmdTagSet;
  delete pCmdTknVec;
  delete pTagTknVec;

  m_pDataStack->vPushInt(true);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* api::reapJobs -- reap retired jobs                                         */
/*----------------------------------------------------------------------------*/
void Step::vApiReapJobs(void)
{
  ENTRY();
  
  i8 iForce = (i8)m_pDataStack->iPopBol(__func__);

  //Api::vReapJobsOnce(iForce);
  gpAttachMap->pJobs()->vReapJobsOnce(iForce);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* api::spawnJob -- spawn the job on the stack                                */
/*----------------------------------------------------------------------------*/
void Step::vApiSpawnJob(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  Task* pTask = pJob->pTask();
  RASSERT(! pTask);
//  if ( pTask) {
//    vTrace("****Job: %s has a task in api::spawnJob\n"_Stz,pJob->szJobNm());
//    RASSERT(QK_NONE == pTask->eQueue());
//    RASSERT(TR_NONE == pTask->eTransition());
//  }
  
  pJob->vTaskCreate();
  Scheduler::vTaskApi_Add(pJob->pTask());

  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
