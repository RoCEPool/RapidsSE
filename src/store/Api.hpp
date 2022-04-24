/*============================================================================*/
/* Api.hpp - Engine Api                                                       */
/*============================================================================*/
#pragma once

#include "eAC.h"

namespace STORE
{
  class Api
  {
  public:
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // external API functions
  
  static Job* pAllocJob(Stz* szJobNm);

  static void vExecuteJob(Job* pJob, TagSet* pCmdTagSet);
  static void vExtractCmdPaths(TokenVec* pCmdTknVec, TagSet* pCmdTagSet);
  static bool bExtractCmdTags(TokenVec* pCmdTknVec, TagSet* pCmdTagSet);
  static void vForkDemon(Job*& rpJob);
  static void vForkJob(Job*& rpJob, Op* pResponseOp);
  static eEC  eParseCmd(TokenVec* pCmdTknVec, TagSet* pCmdTagSet);
  static eEC  ePrepareJob(Job* pJob, TokenVec* pCmdTknVec, TagSet* pCmdTagSet);
  static void vReapableJob(Job*& rpJob);

  static void vSubmitJob(Job* pJob);
  static void vWaitJob(Job* pJob);
 
  static void vReapAllJobs(bool bForce = false);
  static void vReapJobsOnce(bool bForce = false);

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

  static void vSpawnTask_receivedJob(Job* pJob);
      
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
