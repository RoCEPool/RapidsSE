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
  
  static Job* pAllocJob(Stz* szJobNm, JobId tParentId, JobId tOriginId, JobId tDistId);

  static eEC  eForkJob(JobId& rtChildJobId, Job* pParentJob,
                       TokenVec* pCmdTknVec, TagSet* pCmdTagSet,
                       Job*& rpChildJob_tbr);

  static Job* pNewGenericJob(Stz* szJobNm, JobId tParentId, JobId tOriginId, JobId tDistId);

  static void vReapableJob(Job*& rpJob);
  static void vReapAllJobs(bool bForce = false);
  static void vReapJobsOnce(bool bForce = false);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  static void vScheduleNextPhase(Job* pJob, Task* pActiviating2PcTask);
    
  // internal
  
  static void vCallEngineJob(Stz* szJobNm, Stz* szEntryPt, bool bAddAxioms);

  static void vCallJobImmediate(Job* pImmediateJob);

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


  static bool bReapJob(void);


  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
