/*============================================================================*/
/* Api.hpp - Engine Api                                                       */
/*============================================================================*/
#pragma once

namespace Interface
{
  class Api
  {
  public:

  static Session* pAllocSession(void);
  static void     vFreeSession(Session* pSession);
  
  static void vIncStatistics(eAC eApiCall);
  static void vMonitorPortrait(s48K& sPortrait);

  static void vScheduleNextPhase(Job* pJob, Task* pActiviating2PcTask);
  
  // external
  static bool bSpawnJob_connector(Job* pJob);
  static bool bSpawnJob_prompt(Job* pJob);
  
  // internal
  static void vSpawnJob_op(Job* pJob);
  static void vSpawnJob_received(Job* pJob);
  static void vSpawnJob_service(Job* pJob);
  static void vSpawnJob_system(Job* pJob);
  static void vSpawnJobAndWait(Job* pJob);
  
  static void vSpawnJobDedicated(Job* pJob);
  static eEC  eValidateSession(i8 iTrace, Session* pSession, eAC eApiCall);
  
  //! TO GO
  static bool bReseedJob(Session* pSession, Job* pJob);
  
  
  static eEC  ePseudoApi1_PreJob(Job** ppJob);
  static void vPseudoApi2_SpawnJob(Job* pJob);
  static void vPseudoApi3_WaitCompletion(Job* pJob);
  static void vPseudoApi4_PostJob(Job* pJob);
  static void vPseudoApi5_DeleteJob(Job** ppJob);

  static JobClass* aFindJobClass(TokenVec* pCmdTknVec, TagSet* pTagSet);

  private:
  
  static void vCreateTaskForJob(Job* pJob);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
