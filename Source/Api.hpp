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
  static void vFreeSession(Session* pSession);

  static void vDeleteJob(Job** ppJob);
  static void vExecuteJob(Job* pJob);
  static eEC  ePrepareJob(Job**     ppJob,
                          Session* pSession,
                          TokenVec* pCmdTknVec,
                          TokenVec* pLitTknVec,
                          TagSet*   pTagSet,
                          ReqTuple* axReqTuple);

  // external
  static bool bSpawnJob_connector(Job* pJob);
  static bool bSpawnJob_session(Job* pJob);
  
  // internal
  static void vSpawnJob_dedicated(Job* pJob);
  static void vSpawnJob_op(Job* pJob);
  static void vSpawnJob_received(Job* pJob);
  static void vSpawnJob_service(Job* pJob);
  static void vSpawnJob_system(Job* pJob);
  
  static void vSpinTillAllJobsCompleted(void);
  static eEC  eValidateSession(i8 iTrace, Session* pSession, eAC eApiCall);
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  static void vIncStatistics(eAC eApiCall);
  static void vMonitorPortrait(s48K& sPortrait);
  static void vScheduleNextPhase(Job* pJob, Task* pActiviating2PcTask);
  

  static void vMutexLock(Session* pSession, Job* pJob);
  static void vMutexUnlock(Session* pSession, Job* pJob);
  static void vMutexWaitCond(Session* pSession, Job* pJob);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  static bool bShortForm(TokenVec* pCmdTknVec);
  static bool bExpandToLongForm(TokenVec* pCmdTknVec);
  static void vPopulateTagSet(TagSet* pTagSet, TokenVec* pTknVec);

  private:
  
  static void vCreateTaskForJob(Job* pJob);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
