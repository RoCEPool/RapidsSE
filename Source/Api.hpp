/*============================================================================*/
/* Api.hpp - Engine Api                                                       */
/*============================================================================*/
#pragma once

namespace Interface
{
  class Api
  {
  public:

  static Session* pAllocSession(eSK eSession);
  static void     vFreeSession(Session* pSession);

  static void     vExecuteJob(Session* pSession, Job* pJob);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  static eEC  eJob_1_PreJob(Job**     ppJob,
                            TokenVec* pCmdTknVec,
                            TokenVec* pLitTknVec,
                            TagSet*   pTagSet,
                            ReqTuple* axReqTuple);
  static void vJob_2_SpawnJob_prompt(Job* pJob, bool bMustBeLocked=true);
  static void vJob_2_SpawnJob_service(Job* pJob);
  static void vJob_2_SpawnJob_dedicated(Job* pJob);
  static void vJob_3_PostJob(Job* pJob);
  static void vJob_4_DeleteJob(Job** ppJob);

  static bool bShortForm(TokenVec* pCmdTknVec);
  static bool bExpandToLongForm(TokenVec* pCmdTknVec);

  static bool bPassesLowMem(void);

  static void vPopulateTagSet(TagSet* pTagSet, TokenVec* pTknVec);

  static void vMutexLock(Session* pSession, Job* pJob);
  static void vMutexUnlock(Session* pSession, Job* pJob);
  static void vMutexWaitCond(Session* pSession, Job* pJob);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


  static void vIncStatistics(eAC eApiCall);
  static void vMonitorPortrait(s48K& sPortrait);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  static void vScheduleNextPhase(Job* pJob, Task* pActiviating2PcTask);
  
  // external
  static bool bSpawnJob_connector(Job* pJob);
  static bool bSpawnJob_prompt(Job* pJob);
  
  // internal
  static void vSpawnJob_dedicated(Job* pJob);
  static void vSpawnJob_op(Job* pJob);
  static void vSpawnJob_received(Job* pJob);
  static void vSpawnJob_service(Job* pJob);
  static void vSpawnJob_system(Job* pJob);
  static void vSpawnJobAndWait(Job* pJob);
  
  static eEC  eValidateSession(i8 iTrace, Session* pSession, eAC eApiCall);
  
  static void vWaitForAllJobsToComplete(void);
    
  //! TO GO
  static void vSpawnJobDedicated(Job* pJob);
  static bool bReseedJob(Session* pSession, Job* pJob);
  
  
  private:
  
  static void vCreateTaskForJob(Job* pJob);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
