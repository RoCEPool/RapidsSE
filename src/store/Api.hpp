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

  static eEC  eForkJob(JobId& rtChildJobId, Job* pParentJob,
                       TokenVec* pCmdTknVec, TagSet* pCmdTagSet,
                       Job*& rpChildJob_tbr);

  
  static void vReapableJob(Job*& rpJob);
  static void vReapAllJobs(bool bForce = false);
  static void vReapJobsOnce(bool bForce = false);

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
