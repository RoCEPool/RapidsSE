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
  vAddStep("api::parseCommand"_Stz,    &Step::vApiParseCommand);
  vAddStep("api::scheduleTask"_Stz,    &Step::vApiScheduleTask);

  vAddStep("api::forkChild"_Stz,       &Step::vApiForkChild);
  vAddStep("api::forkDemon"_Stz,       &Step::vApiForkDemon);
  vAddStep("api::forkChild2PC"_Stz,    &Step::vApiForkChild2PC);

  vAddStep("api::deleteJob"_Stz,       &Step::vApiDeleteJob);
  vAddStep("api::reapableJob@"_Stz,    &Step::vApiReapableJobFetch);
  vAddStep("api::reapableJobIsOriginator"_Stz,      &Step::vApiReapableIsOriginator);
  vAddStep("api::reapableJobMetricsEnabled"_Stz,    &Step::vApiReapableJobMetricsEnabled);
  vAddStep("api::reapableJobTags@"_Stz,&Step::vApiReapableJobTagsFetch);
  vAddStep("api::spawnJob"_Stz,        &Step::vApiSpawnJob);
}

/*----------------------------------------------------------------------------*/
/* api::vApiParseCommand -- Parses an external command into a list of runnable*/
/* tasks that can be scheduled. Returns true and pushes a TaskList if it      */
/* succeeds. Returns false and pushes an error code and error string if it    */
/* fails.                                                                     */
/*----------------------------------------------------------------------------*/
void Step::vApiParseCommand(void)
{
  ENTRY();
  eEC eErrCode = EC_NONE;
  s2K sErrStr;

  s48K sCmd;
  m_pDataStack->vPopStr(sCmd,__func__);

  s48K sCmdTagStr;
  m_pDataStack->vPopStr(sCmdTagStr,__func__);

  TaskList* pTaskList = Api::pParseCommand(sCmd.szStz(),
                                           sCmdTagStr.szStz(),
                                           this,
                                           eErrCode,
                                           sErrStr);

  if (EC_NONE == eErrCode) {
    m_pDataStack->vPushPtr(pTaskList);
    m_pDataStack->vPushInt(true);
  }
  else {
    m_pDataStack->vPushStz(sErrStr.szStz());
    m_pDataStack->vPushInt(eErrCode);
    m_pDataStack->vPushInt(false);
  }

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Schedules a task that was previously created to be run.                    */
/*----------------------------------------------------------------------------*/
void Step::vApiScheduleTask(void)
{
  ENTRY();

  Task* pChildTask = _pTask(m_pDataStack->pPopPtr(__func__));
  ASSERT(pChildTask);
  ASSERT(pChildTask->eGetClassNo() == CN_TASK);

  // TODO: This should be a suspending call rather than a blocking call
  //       that is is currently. Do not call this from a non-dedicated thread.
  Api::vScheduleTask(pChildTask);

  EXIT();
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
    vTrace("ForkChild (TRANSLITERATE): %s\n"_Stz,sCmd.szStz());
    vTrace("  CmdTags: %s\n"_Stz,sCmdTagStr.szStz());
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // populate the tag set
  TokenVec* pCmdTknVec = AM::TOKENISE::pTokeniseSen(sCmd);
  TokenVec* pTagTknVec = AM::TOKENISE::pTokeniseSen(sCmdTagStr);
  TagSet*   pCmdTagSet = new TagSet;


  TagSet* pJobTags = pOuterJobOfTask()->pJobTags();
  //pJobTags->vContentDump();

  CmdHelper::vPopulateTagSet(pCmdTagSet,pTagTknVec);

  if (pJobTags->bHasName(C_TAG_CCODE))
    pCmdTagSet->vInsert(C_TAG_CCODE);

  if (pJobTags->bHasName(C_TAG_JOBMETRICS))
    pCmdTagSet->vInsert(C_TAG_JOBMETRICS);

  if (pJobTags->bHasName(C_TAG_PIPELINENAME))
    pCmdTagSet->vInsert(C_TAG_PIPELINENAME);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // replace the C_TAG_JOBNM
  Stz* szJobNm = pCmdTknVec->pGet(0)->szChars();
  pCmdTagSet->vReplace(C_TAG_JOBNM,szJobNm);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the C_TAG_PATH
  if (! pCmdTagSet->bHasName(C_TAG_PATH)) {
    if (m_pJob)
      pCmdTagSet->vReplace(C_TAG_PATH,m_pJob->pJobTags()->szValue(C_TAG_PATH));
    else
      pCmdTagSet->vReplace(C_TAG_PATH,C_SE_PATH_ROOT);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // fork the child job

  JobId tParentId;
  JobId tOriginId;
  JobId tDistId;
  Job* pOuterJob = pOuterJobOfTask();
  tParentId = pOuterJob->tJobId();
  RASSERT(tOriginId.bIsUnknown());
  RASSERT(tDistId.bIsUnknown());

  JobId tChildJobId;
  bool bIsPipeline;

  eEC eError = Api::eForkJobChild(tChildJobId,
                                   pCmdTknVec,pCmdTagSet,
                                   tParentId,tOriginId,tDistId,
                                   pOuterJob,
                                   true,
                                   bIsPipeline
  );

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cleanup
  delete pCmdTagSet;
  delete pCmdTknVec;
  delete pTagTknVec;

  if (EC_NONE == eError)
    m_pDataStack->vPushInt(true);
  else {
    m_pDataStack->vPushInt(eError);
    m_pDataStack->vPushInt(false);
  }

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* api::forkDemon --                                                          */
/*----------------------------------------------------------------------------*/
void Step::vApiForkDemon(void)
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
  // populate the tag set
  TokenVec* pCmdTknVec = AM::TOKENISE::pTokeniseSen(sCmd);
  TokenVec* pTagTknVec = AM::TOKENISE::pTokeniseSen(sCmdTagStr);
  TagSet*   pCmdTagSet = new TagSet;
  CmdHelper::vPopulateTagSet(pCmdTagSet,pTagTknVec);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // replace the C_TAG_JOBNM
  Stz* szJobNm = pCmdTknVec->pGet(0)->szChars();
  pCmdTagSet->vReplace(C_TAG_JOBNM,szJobNm);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the C_TAG_PATH
  if (! pCmdTagSet->bHasName(C_TAG_PATH)) {
    if (m_pJob)
      pCmdTagSet->vReplace(C_TAG_PATH,m_pJob->pJobTags()->szValue(C_TAG_PATH));
    else
      pCmdTagSet->vReplace(C_TAG_PATH,C_SE_PATH_ROOT);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // fork the child job

  JobId tParentId;
  JobId tOriginId;
  JobId tDistId;
  Job* pOuterJob = pOuterJobOfTask();
  tParentId = pOuterJob->tJobId();
  RASSERT(tOriginId.bIsUnknown());
  RASSERT(tDistId.bIsUnknown());

  JobId tChildJobId;
  bool bIsPipeline;
  eEC eError = Api::eForkJobChild(tChildJobId,
                                  pCmdTknVec,pCmdTagSet,
                                  tParentId,tOriginId,tDistId,
                                  pOuterJob,
                                  false,
                                  bIsPipeline
  );

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cleanup
  delete pCmdTagSet;
  delete pCmdTknVec;
  delete pTagTknVec;

  if (EC_NONE == eError) {
    m_pDataStack->vPushInt(tChildJobId.iAsI8());
    m_pDataStack->vPushInt(true);
  }
  else {
    m_pDataStack->vPushInt(eError);
    m_pDataStack->vPushInt(false);
  }

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
    vTrace("ForkChild2PC: %s\n"_Stz,sCmd.szStz());
    vTrace("  CmdTags: %s\n"_Stz,sCmdTagStr.szStz());
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  TokenVec* pCmdTknVec = AM::TOKENISE::pTokeniseSen(sCmd);
  TokenVec* pTagTknVec = AM::TOKENISE::pTokeniseSen(sCmdTagStr);
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

  eEC eError = Api::eForkJob2PC(tChildJobId,
                             pCmdTknVec,pCmdTagSet,
                             tParentId,tOriginId,tDistId,
                             pOuterJob
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
/* api::deleteJob -- delete a job                                             */
/*----------------------------------------------------------------------------*/
void Step::vApiDeleteJob(void)
{
  ENTRY();

  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));

  delete pJob;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* api::reapableJob@ -- fetch a reapable job                                  */
/*----------------------------------------------------------------------------*/
void Step::vApiReapableJobFetch(void)
{
  ENTRY();

  Job* pJob = gpAttachMap->pJobs()->pReapableJob();

  m_pDataStack->vPushPtr(pJob);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* api::reapableJobIsOriginator@ -- is reapable job an originator job?        */
/*----------------------------------------------------------------------------*/
void Step::vApiReapableIsOriginator(void)
{
  ENTRY();

  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));

  m_pDataStack->vPushInt(pJob->bOriginatorJob());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* api::reapableJobMetricsEnabledr -- is reapable job metrics enabled?        */
/*----------------------------------------------------------------------------*/
void Step::vApiReapableJobMetricsEnabled(void)
{
  ENTRY();

  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));

  m_pDataStack->vPushInt(pJob->bJobMetrics());

  EXIT();
}
/*----------------------------------------------------------------------------*/
/* api::reapableJobTags@ -- fetch tagSet of reapable job                      */
/*----------------------------------------------------------------------------*/
void Step::vApiReapableJobTagsFetch(void)
{
  ENTRY();

  TagSet* pTagSet = nullptr;
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));

  m_pDataStack->vPushPtr(pJob->pJobTags());

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
  /*
  RASSERT(! pTask);
  //  if ( pTask) {
  //    vTrace("****Job: %s has a task in api::spawnJob\n"_Stz,pJob->szJobNm());
  //    RASSERT(QK_NONE == pTask->eQueue());
  //    RASSERT(TR_NONE == pTask->eTransition());
  //  }

  pJob->vTaskCreate();

 */

  if ( ! pTask )
    pJob->vTaskCreate();

  Scheduler::vTaskApi_Add(pJob->pTask());

  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
