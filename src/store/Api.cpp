/*============================================================================*/
/* Api.cpp -- Api functions                                                   */
/*============================================================================*/

#include "BASE.h"

#include "eYX.h"

#include "ExtendedText.hpp"
#include "Api.hpp"
#include "Job.hpp"
#include "JobList.hpp"
#include "Jobs.hpp"
#include "Op.hpp"
#include "LogicUnit.hpp"
#include "Procedure.hpp"
#include "Output.hpp"
#include "EventHandlers.hpp"
#include "Grid.hpp"
#include "AttachMap.hpp"

#include "TagConstants.h"

#include "SP_TagSet.hpp"
#include "CommandParser.hpp"
#include "SqlException.hpp"

extern AttachMap*   gpAttachMap;
extern BufferPool*  gpBufferPool;
extern CmdLineArgs   gxCmdLineArgs;

extern pthread_key_t giTLS_ThreadKey;
extern pthread_key_t giTLS_TaskKey;
extern pthread_key_t giTLS_HandlerKey;
extern pthread_key_t giTLS_StepKey;
extern pthread_key_t gbTLS_FaultInjection;
extern pthread_key_t giTLS_FaultPoint;
extern pthread_key_t giTLS_CurrentPoint;

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Parse a command into one or more tasks. Returns a tasklist if successful.  */
/* Otherwise returns null and sets the error code and string.                 */
/*----------------------------------------------------------------------------*/
TaskList* Api::pParseCommand(Stz* szCommand, Stz* szCommandTags, Task* pParentTask,
                             eEC& eErrCode, s2K& sErrStr)
{
  // Notes:
  //
  // Note that the szCommand being passed in may contain multiple statements to
  // be executed. e.g. "lstbl; SELECT * FROM t1; DROP TABLE t1;".
  // A separate task will be created for each statement.
  // Similarly, one single command might be implemented as multiple separate
  // commands. e.g. "run _myfile.sql;". This outer run command is replaced with
  // the specific inner commands that are actually executed.
  //
  // The purpose of this API is to:
  //   (1) Provide command translation;
  //         e.g. [SELECT * FROM t1;] gets translated into the command
  //         [sqlCompiler "SELECT * FROM t1;"]
  //
  //   (2) Provide more expressive command parsing & command definitions;
  //         Currently the parsing of commands is limited by the current tokenising
  //         and parsing mechanisms, which limit the expressiveness of how
  //         commands defined.
  //         e.g. support names in double quotes & backticks, command options
  //         that aren't specified in tag syntax, etc.
  //
  //   (3) Better report command syntax errors;
  //
  // Objective #2 means that we cannot directly reuse some of the existing code that
  // prepares jobs based on a vector of tokens.

  Job* pParentJob = pParentTask->pJob();

  // First create a common tag set that will be copied into the tag set for
  // each job.
  TagSet*   pCommonTagSet = new TagSet;

  // Now we tokenise the command tags passed in and populate the common command tag set.
  TokenVec* pCommonTagTknVec = AM::TOKENISE::pTokeniseSen(szCommandTags);
  CmdHelper::vPopulateTagSet(pCommonTagSet, pCommonTagTknVec);
  delete pCommonTagTknVec;
  pCommonTagTknVec = nullptr;

  // We add to this common tag set any inherited tags from the current job
  TagSet* pParentJobTags = pParentTask->pOuterJobOfTask()->pJobTags();

  if (pParentJobTags->bHasName(C_TAG_CCODE))        pCommonTagSet->vInsert(C_TAG_CCODE);
  if (pParentJobTags->bHasName(C_TAG_JOBMETRICS))   pCommonTagSet->vInsert(C_TAG_JOBMETRICS);
  if (pParentJobTags->bHasName(C_TAG_PIPELINENAME)) pCommonTagSet->vInsert(C_TAG_PIPELINENAME);

  // Tokenise and parse the command string using ANTLR.
  // In the process of this we also resolve a command to a pipeline using
  // the dictionary, and substitute pipeline arguments appropriately.
  CommandParser* pCommandParser = new CommandParser(pCommonTagSet, pParentTask);

  TaskList* pTaskList;

  try {
    pTaskList = pCommandParser->pParseCommand(szCommand);

  } catch (Parser::SyntaxException& e) {
    delete pCommonTagSet;
    delete pCommandParser;
    eErrCode = e.eErrorCode();

    if (e.szFile() && !e.szFile()->bIsEmpty()) {
      sErrStr.vSprint("File: %s; Line: %lld; Column: %lld; "_Stz, e.szFile(), e.iLine(), e.iColumn());
    } else {
      sErrStr.vSprint("Line: %lld; Column: %lld; "_Stz, e.iLine(), e.iColumn());
    }

    sErrStr.vConcat(e.szError(), sErrStr.nFreeBytes() - 1);
    return nullptr;

  } catch (Parser::SemanticException& e) {
    delete pCommonTagSet;
    delete pCommandParser;
    eErrCode = e.eErrorCode();
    sErrStr.vCopy(e.szError(), sErrStr.nFreeBytes() - 1);
    return nullptr;
  }

  ASSERT(pTaskList);
  ASSERT(pTaskList->nTasks() > 0);
  delete pCommonTagSet;
  delete pCommandParser;

  // Do any final adjustments to each command's tag set.
  Task* pTask;
  pTaskList->vGrabLock();
  pTask = pTaskList->pPeekTask();
  pTaskList->vDropLock();

  while (pTask) {
    TagSet* pCmdTagSet = pTask->pJob()->pJobTags();

    // Ensure the tag path is set
    if (!pCmdTagSet->bHasName(C_TAG_PATH)) {
      if (pParentJob) {
        pCmdTagSet->vReplace(C_TAG_PATH, pParentJob->pJobTags()->szValue(C_TAG_PATH));

      } else {
        pCmdTagSet->vReplace(C_TAG_PATH, C_SE_PATH_ROOT);
      }
    }

    pTask = pTask->pNextItem();
  }

  if ( AM::pGrid() && ! AM::pGrid()->bHealthy()) {
    eErrCode = EC_NODE_LOSS;
    delete pTaskList;

    return nullptr;
  }

  if (! AM::PROCESS::bSeRunning()) {
    eErrCode = EC_NODE_LOSS;
    delete pTaskList;
    return nullptr;
  }


  return pTaskList;
}


/*----------------------------------------------------------------------------*/
/* schedule a task to run                                                     */
/*----------------------------------------------------------------------------*/
eEC Api::eScheduleTask(ENGINE::Task* pTask)
{
  // TODO
  // NOTE: While the content of this function is currently the same as
  // vTriggerChildJob(), the aim is eventually make it suspend the parent
  // job instead of blocking the parent job.

  _static_

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  RASSERT(pTask->pJob());
  Job* pChildJob = pTask->pJob();

  RASSERT(pTask->pJob()->pParentJob());
  Job* pParentJob = pTask->pJob()->pParentJob();


  pParentJob->vMutexLock();
  RASSERT(MS_LOCKED == pParentJob->eMutexSide());

  RASSERT(pChildJob->pTask());

  pChildJob->vResponseTimerStart();

  RASSERT(pChildJob->bHasTag(C_TAG_NOWAIT));
  RASSERT(! pChildJob->bHasTag(C_TAG_WAIT));

  Scheduler::vTaskApi_Add(pChildJob->pTask());

  pParentJob->vMutexWait();
  pParentJob->vMutexUnlock();

  // check for error from child job
  eEC eChildError = pParentJob->eChildError();

  if (eChildError != EC_NONE) {
    return eChildError;
  }

  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* Create an unscheduled task from a pipeline command                         */
/*----------------------------------------------------------------------------*/
Task* Api::pCreateTask(Stz* szPipelineCmd, TagSet* pCmdTagSet, JobId tParentId, Stz* szOrigCmd)
{
  // Tokenise the pipeline command
  TokenVec* pCmdTknVec = AM::TOKENISE::pTokeniseSen(szPipelineCmd);

  // Parse the command and copy the command's default tags into pCmdTagSet.
  if (! CmdHelper::bParseCmd(pCmdTknVec, pCmdTagSet)) {
    // All commands that come this way should have been parsed by ANTLR
    // and then resolved to a pipeline statement already. The only exception
    // is if/when the system accepts raw pipeline commands through ANTLR.
    delete pCmdTknVec;
    return nullptr;
  }

  delete pCmdTknVec;
  pCmdTknVec = nullptr;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // verify tags

  // C_TAG_JOBNM
  if (! pCmdTagSet->bHasName(C_TAG_JOBNM)) {
    pCmdTagSet->vDumpSet();
    THROW("Api::pCreateTask: no C_TAG_JOBNM"_Stz);
  }

  // C_TAG_ENTRYPOINT
  if (! pCmdTagSet->bHasName(C_TAG_ENTRYPOINT)) {
    pCmdTagSet->vDumpSet();
    THROW("Api::pCreateTask: no C_TAG_ENTRYPOINT"_Stz);
  }

  // C_TAG_PATH
  if (! pCmdTagSet->bHasName(C_TAG_PATH)) {
    pCmdTagSet->vDumpSet();
    THROW("Api::pCreateTask: no C_TAG_PATH"_Stz);
  }

  // C_TAG_NOWAIT / C_TAG_WAIT
  if ((! pCmdTagSet->bHasName(C_TAG_NOWAIT)) &&
      (! pCmdTagSet->bHasName(C_TAG_WAIT))) {
    pCmdTagSet->vDumpSet();
    THROW("Api::pCreateTask: neither C_TAG_NOWAIT or C_TAG_WAIT"_Stz);
  }
  if (pCmdTagSet->bHasName(C_TAG_NOWAIT) &&
      pCmdTagSet->bHasName(C_TAG_WAIT)) {
    pCmdTagSet->vDumpSet();
    THROW("Api::pCreateTask: both C_TAG_NOWAIT and C_TAG_WAIT"_Stz);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Set the original statement in this pipeline job
  pCmdTagSet->vAppend(C_TAG_STMT, szOrigCmd);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create the child job

  Stz* szJobNm = pCmdTagSet->szValue(C_TAG_JOBNM);
  RASSERT(szJobNm);

  JobId tOriginId;
  JobId tDistId;

  Job* pChildJob = new Job(szJobNm, tParentId, tOriginId, tDistId, pCmdTagSet);
//  bIsPipeline = pChildJob->bIsPipeline();

//  vTrace("eForkJobChild: %04lld.%04lld\n"_Stz,
//         pChildJob->tJobId().uNodeId(),
//         pChildJob->tJobId().uJobNo());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // save child job pointer in thread object till its task is assigned
  // and accessible via TLS

  Thread* pThread = Thread::pThisTLSThread();
  if(pThread) {
    pThread->vObject0(pChildJob);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set updateParent flag in child

  pChildJob->vUpdateParent(true);

  // return the jobId
//  rtChildJobId = pChildJob->tJobId();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy CmdTagSet to the child job tags

  TagSet* pJobTagSet = pChildJob->pJobTags();
  pJobTagSet->vCopy(pCmdTagSet);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // setup the child job

  // sCmd
  pChildJob->vCmdText(szPipelineCmd);

  // szEntry
  Stz* szEntry = pJobTagSet->szValue(C_TAG_ENTRYPOINT);
  pChildJob->vEntryPoint(szEntry);

  // szFolderPath
  Stz* szFolderPath = pJobTagSet->szValue(C_TAG_PATH);
  pChildJob->vFolderPath(szFolderPath);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create the task for the child job
  pChildJob->vTaskCreate();
  RASSERT(pChildJob->pTask());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // now the child job has a task and accessible via TLS,
  // we can remove its pointer from this thread

  if(pThread) {
    pThread->vObject0(nullptr);
  }

  return pChildJob->pTask();
}


/*----------------------------------------------------------------------------*/
/* delete root job                                                           */
/*----------------------------------------------------------------------------*/
void Api::vDeleteRootJob(Job*& rpRootJob)
{
  _static_

  delete rpRootJob;

  rpRootJob = nullptr;
}

/*----------------------------------------------------------------------------*/
/* create a 2PC job                                                           */
/*----------------------------------------------------------------------------*/
Job* Api::pCreateJob2PC(Job* pParentJob, Stz* sCmd)
{
  _static_

  Job* p2PCJob;

  JobId tParentId;
  JobId tOriginId;
  JobId tDistId;
  tParentId = pParentJob->tJobId();
  tOriginId = pParentJob->tOriginId();
  tDistId   = pParentJob->tDistId();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // assertions

  RASSERT(! tParentId.bIsUnknown());
  RASSERT(pParentJob);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // prepare job

  TokenVec* pCmdTknVec = AM::TOKENISE::pTokeniseSen(sCmd);
  TagSet*   pCmdTagSet = new TagSet;
  pCmdTagSet->vAppend(C_TAG_NOWAIT, 1);

  bool b = CmdHelper::bExtractCmdTags(pCmdTagSet,pCmdTknVec);
  UNUSED(b);

  sSTMT sMaybePipelineSmt;
  if (Procedure::bTranslateToPipeline(sMaybePipelineSmt.pString(),pCmdTknVec)) {
    pCmdTknVec->vSoftReset();
    AM::TOKENISE::vTokeniseSen(pCmdTknVec,sMaybePipelineSmt);
  }

  if (! CmdHelper::bParseCmd(pCmdTknVec,pCmdTagSet)) {
    RASSERT(0);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // verify minimum needed tags

  if (! pCmdTagSet->bHasName(C_TAG_JOBNM)) {
    pCmdTagSet->vDumpSet();
    THROW("Api::eForkJob2PC: no C_TAG_JOBNM"_Stz);
  }
  if (! pCmdTagSet->bHasName(C_TAG_ENTRYPOINT)) {
    pCmdTagSet->vDumpSet();
    THROW("Api::eForkJob2PC: no C_TAG_ENTRYPOINT"_Stz);
  }
  if (! pCmdTagSet->bHasName(C_TAG_PATH)) {
    pCmdTagSet->vDumpSet();
    THROW("Api::eForkJob2PC: no C_TAG_PATH"_Stz);
  }

  if ((! pCmdTagSet->bHasName(C_TAG_NOWAIT)) &&
      (! pCmdTagSet->bHasName(C_TAG_WAIT))) {
    pCmdTagSet->vDumpSet();
    THROW("Api::eForkJob2PC: neither C_TAG_NOWAIT or C_TAG_WAIT"_Stz);
  }
  if (pCmdTagSet->bHasName(C_TAG_NOWAIT) &&
      pCmdTagSet->bHasName(C_TAG_WAIT)) {
    pCmdTagSet->vDumpSet();
    THROW("Api::eForkJob2PC: both C_TAG_NOWAIT and C_TAG_WAIT"_Stz);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Stz* szJobNm = pCmdTagSet->szValue(C_TAG_JOBNM);
  RASSERT(szJobNm);

  p2PCJob = new Job(szJobNm,tParentId,tOriginId,tDistId);

  TagSet* pJobTagSet = p2PCJob->pJobTags();
  pJobTagSet->vCopy(& gxCmdLineArgs.m_xSysArgs.m_oSysTags);
  pJobTagSet->vCopy(pCmdTagSet);

  p2PCJob->vCmdText(sCmd);

  Stz* szEntry = pJobTagSet->szValue(C_TAG_ENTRYPOINT);
  p2PCJob->vEntryPoint(szEntry);

  Stz* szFolderPath = pJobTagSet->szValue(C_TAG_PATH);
  p2PCJob->vFolderPath(szFolderPath);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  p2PCJob->vTaskCreate();


  delete pCmdTagSet;
  delete pCmdTknVec;

  return p2PCJob;
}

/*----------------------------------------------------------------------------*/
/* fork a child job for given command                                         */
/*----------------------------------------------------------------------------*/
eEC Api::eForkJobChild(JobId& rtChildJobId,
                        TokenVec* pCmdTknVec, TagSet* pCmdTagSet,
                        ExtendedText* pCodeBlock,
                        JobId tParentId, JobId tOriginId, JobId tDistId,
                        Job*  pParentJob,
                        bool  bUpdateParent,
                        bool& bIsPipeline)
{
  _static_

  RASSERT(! pCodeBlock);

  if ( AM::pGrid() &&
      ! AM::pGrid()->bHealthy()) {
    return EC_NODE_LOSS;
  }

  if (! AM::PROCESS::bSeRunning()) {
    return EC_NODE_LOSS;
  }

            Job* pChildJob;
  rtChildJobId = C_UNKNOWN;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // should have a parent (deputy root job)

  RASSERT(! tParentId.bIsUnknown());
  RASSERT(pParentJob);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // prepare the new job

  CmdHelper::vExtractCmdPaths(pCmdTagSet,pCmdTknVec);

  bool b = CmdHelper::bExtractCmdTags(pCmdTagSet,pCmdTknVec);
  UNUSED(b);

  sSTMT sMaybePipelineSmt;
  if (Procedure::bTranslateToPipeline(sMaybePipelineSmt.pString(),pCmdTknVec)) {
    pCmdTknVec->vSoftReset();
    AM::TOKENISE::vTokeniseSen(pCmdTknVec,sMaybePipelineSmt);
  }
#ifdef WITH_OOM_FAULTINJECTION
  PsxThd::vTlsSet(giTLS_CurrentPoint,(void*)1);
#endif
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // parse the command

  if (! CmdHelper::bParseCmd(pCmdTknVec,pCmdTagSet)) {
    rtChildJobId = C_UNKNOWN;
    return EC_INVALID_COMMAND;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // verify tags

  // C_TAG_JOBNM
  if (! pCmdTagSet->bHasName(C_TAG_JOBNM)) {
    pCmdTagSet->vDumpSet();
    THROW("Api::eForkJob: no C_TAG_JOBNM"_Stz);
  }

  // C_TAG_ENTRYPOINT
  if (! pCmdTagSet->bHasName(C_TAG_ENTRYPOINT)) {
    pCmdTagSet->vDumpSet();
    THROW("Api::eForkJob: no C_TAG_ENTRYPOINT"_Stz);
  }

  // C_TAG_PATH
  if (! pCmdTagSet->bHasName(C_TAG_PATH)) {
    pCmdTagSet->vDumpSet();
    THROW("Api::eForkJob: no C_TAG_PATH"_Stz);
  }

  // C_TAG_NOWAIT / C_TAG_WAIT
  if ((! pCmdTagSet->bHasName(C_TAG_NOWAIT)) &&
      (! pCmdTagSet->bHasName(C_TAG_WAIT))) {
    pCmdTagSet->vDumpSet();
    THROW("Api::eForkJobChild: neither C_TAG_NOWAIT or C_TAG_WAIT"_Stz);
  }
  if (pCmdTagSet->bHasName(C_TAG_NOWAIT) &&
      pCmdTagSet->bHasName(C_TAG_WAIT)) {
    pCmdTagSet->vDumpSet();
    THROW("Api::eForkJobOld: both C_TAG_NOWAIT and C_TAG_WAIT"_Stz);
  }


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create the child job

  Stz* szJobNm = pCmdTagSet->szValue(C_TAG_JOBNM);
  RASSERT(szJobNm);

  pChildJob = new Job(szJobNm,tParentId,tOriginId,tDistId,pCmdTagSet);
  bIsPipeline = pChildJob->bIsPipeline();

//  vTrace("eForkJobChild: %04lld.%04lld\n"_Stz,
//         pChildJob->tJobId().uNodeId(),
//         pChildJob->tJobId().uJobNo());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // save child job pointer in thread object till its task is assigned
  // and accessible via TLS

  Thread* pThread = Thread::pThisTLSThread();
  if(pThread)
    pThread->vObject0(pChildJob);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set updateParent flag in child

  pChildJob->vUpdateParent(bUpdateParent);

  // return the jobId
  rtChildJobId = pChildJob->tJobId();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy CmdTagSet to the child job tags

  TagSet* pJobTagSet = pChildJob->pJobTags();
  pJobTagSet->vCopy(& gxCmdLineArgs.m_xSysArgs.m_oSysTags);
  pJobTagSet->vCopy(pCmdTagSet);

  if (pJobTagSet->bHasName(C_SHOW_CMD_TAGS)) {
    pJobTagSet->vDumpSet();
    vDebugPoint();
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // setup the child job

  // sCmd
  sSTMT sCmd;
  pCmdTknVec->vVecToString(sCmd,0,true);
  pChildJob->vCmdText(sCmd);

  // szEntry
  Stz* szEntry = pJobTagSet->szValue(C_TAG_ENTRYPOINT);
  pChildJob->vEntryPoint(szEntry);

  // szFolderPath
  Stz* szFolderPath = pJobTagSet->szValue(C_TAG_PATH);
  pChildJob->vFolderPath(szFolderPath);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create the task for the child job
  pChildJob->vTaskCreate();
  RASSERT(pChildJob->pTask());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // now the child job has a task and accessible via TLS,
  // we can remove its pointer from this thread
  
  if(pThread)
    pThread->vObject0(nullptr);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // lock the mutex
  pParentJob->vMutexLock();
  RASSERT(MS_LOCKED == pParentJob->eMutexSide());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // start the timer of the child job
  pChildJob->vResponseTimerStart();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // add task to scheduler
  Scheduler::vTaskApi_Add(pChildJob->pTask());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // wait for signal from child job
  // if C_TAG_WAIT parent gets signalled when child job completes
  // if C_TAG_NOWAIT parent gets signalled when pretests/ compilation are done

  pParentJob->vMutexWait();
  pParentJob->vMutexUnlock();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // check for error from child job
  eEC eChildError = pParentJob->eChildError();

  if (eChildError != EC_NONE) {
    return eChildError;
  }

  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* fork a child job for given command (2PC)                                   */
/*----------------------------------------------------------------------------*/
eEC Api::eForkJob2PC(JobId& rtChildJobId,
                     TokenVec* pCmdTknVec, TagSet* pCmdTagSet,
                     JobId tParentId, JobId tOriginId, JobId tDistId,
                     Job* pParentJob)
{
  _static_

  Job* pChildJob;
  rtChildJobId = C_UNKNOWN;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // assertions

  RASSERT(! tParentId.bIsUnknown());
  RASSERT(pParentJob);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // prepare job

  CmdHelper::vExtractCmdPaths(pCmdTagSet,pCmdTknVec);

  bool b = CmdHelper::bExtractCmdTags(pCmdTagSet,pCmdTknVec);
  UNUSED(b);

  sSTMT sMaybePipelineSmt;
  if (Procedure::bTranslateToPipeline(sMaybePipelineSmt.pString(),pCmdTknVec)) {
    pCmdTknVec->vSoftReset();
    AM::TOKENISE::vTokeniseSen(pCmdTknVec,sMaybePipelineSmt);
  }

  if (! CmdHelper::bParseCmd(pCmdTknVec,pCmdTagSet)) {
    rtChildJobId = C_UNKNOWN;
    return EC_INVALID_COMMAND;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // verify minimum needed tags

  if (! pCmdTagSet->bHasName(C_TAG_JOBNM)) {
    pCmdTagSet->vDumpSet();
    THROW("Api::eForkJob2PC: no C_TAG_JOBNM"_Stz);
  }
  if (! pCmdTagSet->bHasName(C_TAG_ENTRYPOINT)) {
    pCmdTagSet->vDumpSet();
    THROW("Api::eForkJob2PC: no C_TAG_ENTRYPOINT"_Stz);
  }
  if (! pCmdTagSet->bHasName(C_TAG_PATH)) {
    pCmdTagSet->vDumpSet();
    THROW("Api::eForkJob2PC: no C_TAG_PATH"_Stz);
  }

  if ((! pCmdTagSet->bHasName(C_TAG_NOWAIT)) &&
      (! pCmdTagSet->bHasName(C_TAG_WAIT))) {
    pCmdTagSet->vDumpSet();
    THROW("Api::eForkJob2PC: neither C_TAG_NOWAIT or C_TAG_WAIT"_Stz);
  }
  if (pCmdTagSet->bHasName(C_TAG_NOWAIT) &&
      pCmdTagSet->bHasName(C_TAG_WAIT)) {
    pCmdTagSet->vDumpSet();
    THROW("Api::eForkJob2PC: both C_TAG_NOWAIT and C_TAG_WAIT"_Stz);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Stz* szJobNm = pCmdTagSet->szValue(C_TAG_JOBNM);
  RASSERT(szJobNm);

  pChildJob = new Job(szJobNm,tParentId,tOriginId,tDistId);

  // return the jobId
  rtChildJobId = pChildJob->tJobId();

  TagSet* pJobTagSet = pChildJob->pJobTags();
  pJobTagSet->vCopy(& gxCmdLineArgs.m_xSysArgs.m_oSysTags);
  pJobTagSet->vCopy(pCmdTagSet);

  sSTMT sCmd;
  pCmdTknVec->vVecToString(sCmd,0,true);
  pChildJob->vCmdText(sCmd);

  Stz* szEntry = pJobTagSet->szValue(C_TAG_ENTRYPOINT);
  pChildJob->vEntryPoint(szEntry);

  Stz* szFolderPath = pJobTagSet->szValue(C_TAG_PATH);
  pChildJob->vFolderPath(szFolderPath);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pChildJob->vTaskCreate();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pParentJob->vMutexLock();
  RASSERT(MS_LOCKED == pParentJob->eMutexSide());

  RASSERT(pChildJob->pTask());

  pChildJob->vResponseTimerStart();

  RASSERT(pChildJob->bHasTag(C_TAG_NOWAIT));
  RASSERT(! pChildJob->bHasTag(C_TAG_WAIT));

  Scheduler::vTaskApi_Add(pChildJob->pTask());

  pParentJob->vMutexUnlock();
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* new a dummy/root job                                                       */
/*----------------------------------------------------------------------------*/
Job* Api::pNewRootJob(Stz* szJobNm, JobId tParentId, JobId tOriginId, JobId tDistId)
{
  _static_
  
  Job* pRootJob = new Job(szJobNm,tParentId,tOriginId,tDistId);

  return pRootJob;
}


/*----------------------------------------------------------------------------*/
/* trigger a child (2PC)                                                      */
/*----------------------------------------------------------------------------*/
void Api::vTriggerChildJob(Job* pParentJob, Job* pChildJob)
{
  _static_

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pParentJob->vMutexLock();
  RASSERT(MS_LOCKED == pParentJob->eMutexSide());

  RASSERT(pChildJob->pTask());

  pChildJob->vResponseTimerStart();

  RASSERT(pChildJob->bHasTag(C_TAG_NOWAIT));
  RASSERT(! pChildJob->bHasTag(C_TAG_WAIT));

  Scheduler::vTaskApi_Add(pChildJob->pTask());

  pParentJob->vMutexUnlock();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* call the system boot job to compile the main dictionary                    */
/*----------------------------------------------------------------------------*/
void Api::vCallEngineJob(Stz* szJobNm, Stz* szEntryPt, bool bAddAxioms)
{
  _static_
  
  // this is the main thread
  PsxThd::vMarkNonSeThread();

  JobId tParentId;
  RASSERT(tParentId.bIsUnknown());

  JobId tOriginId;
  RASSERT(tOriginId.bIsUnknown());

  JobId tDistId;
  RASSERT(tDistId.bIsUnknown());

  Job* pRootJob = Api::pNewRootJob("startSystemRoot"_Stz,tParentId,tOriginId,tDistId);
  pRootJob->vOutputCreate();
  pRootJob->vOutputBeginProc();
  pRootJob->vOutputEnabled(true);

  tParentId = pRootJob->tJobId();
  tOriginId = pRootJob->tOriginId();
  tDistId = pRootJob->tDistId();

  Job* pEngineJob   = new Job(szJobNm,tParentId,tOriginId,tDistId);
  Task* pEngineTask = new Task(szJobNm,pEngineJob);
  pEngineJob->vTask(pEngineTask);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     
  if (bAddAxioms) {
    pEngineTask->vAddAxioms();
    pEngineTask->vAddMunged();
  }
  
  pEngineTask->vSetInxPtr(szEntryPt);
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  //vCallJobImmediate(pEngineJob);
  
  TagSet* pJobTags = pEngineJob->pJobTags();
  //pJobTags->vDumpSet();

  pJobTags->vReplace(C_TAG_PATH,"/"_Stz);
  pJobTags->vReplace(C_TAG_WAIT,1);
  
  pEngineJob->vResponseTimerStart();
 
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  //vCallTaskImmediate(pImmediateTask);
  Task* pSaveTask = _pTask(PsxThd::pTlsGet(giTLS_TaskKey));
  
  pEngineTask->vState(TS_EXECUTING);
  
  pEngineTask->vTaskLoop();
  
  PsxThd::vTlsSet(giTLS_TaskKey,pSaveTask);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Api::vDeleteRootJob(pRootJob);

  pEngineTask->vState(TS_RETIRED);
  pEngineTask->vDropTaskLock((Stz*)__func__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (bAddAxioms) {
    AM::HANDLERS::pEventHandlers()->vInitialise();
  }
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
