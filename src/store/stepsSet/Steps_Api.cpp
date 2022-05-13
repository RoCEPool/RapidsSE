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
  vAddStep("api::forkChild"_Stz,    &Step::vApiForkChild);
  vAddStep("api::reapJobs"_Stz,     &Step::vApiReapJobs);
  vAddStep("api::spawnJob"_Stz,     &Step::vApiSpawnJob);
}

/*----------------------------------------------------------------------------*/
/* api::forkChild -- fork child job                                           */
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
    
  TagSet* pCmdTagSet = new TagSet;
  TokenVec* pTagTknVec = TokenVec::pTokeniseSen(sCmdTagStr);
  CmdHelper::vPopulateTagSet(pCmdTagSet,pTagTknVec);

  Stz* szJobNm = pCmdTknVec->pGet(0)->szChars();
  pCmdTagSet->vReplace(C_TAG_JOBNM,szJobNm);

  if (m_pJob) {
//    if (! pCmdTagSet->bHasName(C_TAG_LASTJOBID)) {
//      pCmdTagSet->vReplace(C_TAG_LASTJOBID,m_pJob->tJobId().iAsI8());
//    }
    if (! pCmdTagSet->bHasName(C_TAG_PATH)) {
      pCmdTagSet->vReplace(C_TAG_PATH,
                           m_pJob->pJobTags()->szValue(C_TAG_PATH));
    }
  }
  else {
//    if (! pCmdTagSet->bHasName(C_TAG_LASTJOBID)) {
//      pCmdTagSet->vReplace(C_TAG_LASTJOBID,C_UNKNOWN);
//    }
    if (! pCmdTagSet->bHasName(C_TAG_PATH)) {
      pCmdTagSet->vReplace(C_TAG_PATH,C_SE_PATH_ROOT);
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  JobId tParentId;
  if (m_pJob)
    tParentId = m_pJob->tJobId();

  JobId tOriginId;
  if (m_pJob)
    tOriginId = m_pJob->tOriginId();

  JobId tDistId;
  if (m_pJob)
    tDistId = m_pJob->tDistId();

  JobId tChildJobId;
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
/* api::spawnJob -- spawn the job on the stack                         */
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
