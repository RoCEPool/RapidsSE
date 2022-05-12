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
  vAddStep("api::forkChild_noWait"_Stz,    &Step::vApiForkChild_noWait);
  vAddStep("api::forkChild_wait"_Stz,      &Step::vApiForkChild_wait);
  vAddStep("api::forkGeneric_noWait"_Stz,  &Step::vApiForkGeneric_noWait);
  vAddStep("api::reapJobs"_Stz,            &Step::vApiReapJobs);
  vAddStep("api::spawnJob_noWait"_Stz,     &Step::vApiSpawnJob);
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
  CmdHelper::vPopulateTagSet(pCmdTagSet,pTagTknVec);

  pCmdTagSet->vReplace(C_TAG_NOWAIT,1);

  if (m_pJob) {
    if (! pCmdTagSet->bHasName(C_TAG_LASTJOBID)) {
      pCmdTagSet->vReplace(C_TAG_LASTJOBID,m_pJob->tJobId().iAsI8());
    }
    if (! pCmdTagSet->bHasName(C_TAG_PATH)) {
      pCmdTagSet->vReplace(C_TAG_PATH,
                           m_pJob->pJobTags()->szValue(C_TAG_PATH));
    }
  }
  else {
    if (! pCmdTagSet->bHasName(C_TAG_LASTJOBID)) {
      pCmdTagSet->vReplace(C_TAG_LASTJOBID,C_UNKNOWN);
    }
    if (! pCmdTagSet->bHasName(C_TAG_PATH)) {
      pCmdTagSet->vReplace(C_TAG_PATH,C_SE_PATH_ROOT);
    }
  }

  JobId tChildJobId;
  
  JobId tParentId;
  if (m_pJob)
    tParentId = m_pJob->tJobId();

  JobId tOriginId;
  if (m_pJob)
    tOriginId = m_pJob->tOriginId();

  JobId tDistId;
  if (m_pJob)
    tDistId = m_pJob->tDistId();

  Job* pJob = nullptr;
  eEC eError = Api::eForkJob(tChildJobId,
                             pCmdTknVec,pCmdTagSet,
                             tParentId,tOriginId,tDistId,
                             pJob // TO BE REMOVED
                            );
  RASSERT(EC_NONE == eError);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  delete pCmdTknVec;
  delete pCmdTagSet;
  
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
  CmdHelper::vPopulateTagSet(pCmdTagSet,pTagTknVec);

  
  /*
  if (! pCmdTagSet->bHasName(C_TAG_PATH)) {
    
    if (m_pJob) {
      TagSet* pParentJobTagSet = m_pJob->pJobTags();
      if (! pParentJobTagSet->bHasName(C_TAG_PATH)) {
        pCmdTagSet->vDumpSet();
        pParentJobTagSet->vDumpSet();
        THROW("Step::vApiForkChild_noWait: no C_TAG_PATH"_Stz);
      }
      pCmdTagSet->vReplace(C_TAG_PATH,pParentJobTagSet->szValue(C_TAG_PATH));
    }
    else {
      // this is a standalone op like answerSE                                  FIX!!
      pCmdTagSet->vReplace(C_TAG_PATH,C_SE_PATH_ROOT);
    }
  }
  */
  
  
  if (m_pJob) {
    if (! pCmdTagSet->bHasName(C_TAG_LASTJOBID)) {
      pCmdTagSet->vReplace(C_TAG_LASTJOBID,m_pJob->tJobId().iAsI8());
    }
  }
  else {
    if (! pCmdTagSet->bHasName(C_TAG_LASTJOBID)) {
      pCmdTagSet->vReplace(C_TAG_LASTJOBID,C_UNKNOWN);
    }
  }
  
  pCmdTagSet->vReplace(C_TAG_WAIT,1);
  
  JobId tChildJobId;
  
  
  JobId tParentId;
  tParentId = m_pJob->tJobId();

  JobId tOriginId;
  tOriginId = m_pJob->tOriginId();

  JobId tDistId;
  tDistId = m_pJob->tDistId();

  
  Job* pJob = nullptr;
  eEC eError = Api::eForkJob(tChildJobId,
                             pCmdTknVec,pCmdTagSet,
                             tParentId,tOriginId,tDistId,
                             pJob // TO BE REMOVED
                             );
  RASSERT(EC_NONE == eError);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_pDataStack->vPushInt(true);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* api::forkGeneric_noWait -- fork a pipeline coordination job                */
/*----------------------------------------------------------------------------*/
void Step::vApiForkGeneric_noWait(void)
{
  ENTRY();
  
  s256 sCmd;
  m_pDataStack->vPopStr(sCmd,__func__);
    
  JobId tParentId = m_pJob->tParentId();
  JobId tOriginId = m_pJob->tOriginId();
  JobId tDistId   = m_pJob->tDistId();
  
  Job* pJobGeneric = nullptr;
  JobId tChildJobId;
  
  TokenVec* pCmdTknVec = TokenVec::pTokeniseSen(sCmd);
  TagSet*   pCmdTagSet = new TagSet;

  pCmdTagSet->vReplace(C_TAG_JOBNM,sCmd);
  
  eEC eError = Api::eNewGenericJob(tChildJobId,
                                   pCmdTknVec,pCmdTagSet,
                                   tParentId,tOriginId,tDistId,
                                   pJobGeneric);
  RASSERT(EC_NONE == eError);
  
  delete pCmdTknVec;
  delete pCmdTagSet;
 
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
/* api::spawnJob_noWait -- spawn the job on the stack                         */
/*----------------------------------------------------------------------------*/
void Step::vApiSpawnJob(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  Task* pTask = pJob->pTask();
  RASSERT(! pTask);
//  if ( pTask) {
//    vTrace("****Job: %s has a task in api::spawnJob_noWait\n"_Stz,pJob->szJobNm());
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
