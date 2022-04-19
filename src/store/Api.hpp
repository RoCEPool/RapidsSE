/*============================================================================*/
/* Api.hpp - Engine Api                                                       */
/*============================================================================*/
#pragma once

#include "eAC.h"
#include "eSK.h"

namespace STORE
{
  class Api
  {
  public:
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // external API functions
  
  static Session* pAllocSession(eSK eSession);
  static void vFreeSession(Session* pSession);

  static Job* pAllocJob(Session* pSession);

  static void vExecuteJob(Session* pSession, Job* pJob);
  static void vExtractCmdPaths(TokenVec* pCmdTknVec, TagSet* pCmdTagSet);
  static bool bExtractCmdTags(TokenVec* pCmdTknVec, TagSet* pCmdTagSet);
  static void vForkJob(Session* pSession, Op* pResponseOp, Job*& rpJob);
  static eEC  eParseCmd(TokenVec* pCmdTknVec, TagSet* pCmdTagSet);
  static eEC  ePrepareJob(Session*  pSession, TokenVec* pCmdTknVec, Job*& rpJob);
  static void vReapableJob(Session* pSession, Job*& rpJob);

  static void vSubmitJob(Job* pJob);
  static void vWaitJob(Job* pJob);
 
  static void vReapAllJobs(bool bForce = false);
  static void vReapJobsOnce(bool bForce = false);

  static eEC  eExecuteCommand(Stz* szCmd, Stz* szWithTagStr, Stz* szTagPath);
  static eEC  eForkCommand(Stz* szCmd, Stz* szWithTagStr, Stz* szTagPath,
                           Op* pResponseOp, JobId& rtJobId);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  static void vScheduleNextPhase(Job* pJob, Task* pActiviating2PcTask);
    
  // internal
  
  static void vCallBootSystemJob(void);
  static void vCallStartSystemJob(void);

  static void vCallTaskImmediate(Task* pImmediateTask);

  static void vSpawnTask_op(Op* pop);
  static void vSpawnTask_dedicatedOp(Op* pOp);

  static void vSpawnTask_demonJob(Job* pJob);
  static void vSpawnTask_receivedJob(Job* pJob);
  
  static void vSpinTillAllJobsCompleted(void);
  static eEC  eValidateSession(i8 iTrace, Session* pSession, eAC eApiCall);
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  static void vIncStatistics(eAC eApiCall);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  static bool bShortForm(TokenVec* pCmdTknVec);
  static bool bExpandToLongForm(TokenVec* pCmdTknVec);
  static void vPopulateTagSet(TagSet* pTagSet, TokenVec* pTknVec);

  private:
  
  static void vCreateTaskForOp(Op* pOp);
  static void vResetTaskForJob(Job* pJob);

  static void vMutexLock(Job* pJob);
  static void vMutexUnlock(Job* pJob);
  static void vMutexWaitCond(Job* pJob);

  static bool bReapJob(void);


  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
