/*============================================================================*/
/* Api.hpp - Engine Api                                                       */
/*============================================================================*/
#pragma once

namespace Interface
{
  class Api
  {
  public:
  static void vDeleteJobs(Session* pSession);
  static Job* pExecuteCommand(Session* pSession, Stz* sCommand,
                              bool bPostJob, bool bFromPrompt);
  static void vIncStatistics(eAC eApiCall);
  static void vMonitorPortrait(s48K& sPortrait);
  static bool bReseedJob(Session* pSession, Job* pJob);

  static bool bSpawnJob(Job* pJob);
  static bool bSpawnJob_connector(Job* pJob);
  static bool bSpawnJob_internal(Job* pJob);
  static bool bSpawnJob_op(Job* pJob);
  static bool bSpawnJob_other(Job* pJob);
  static bool bSpawnJob_prompt(Job* pJob);
  static bool bSpawnJob_received(Job* pJob);
  static bool bSpawnJob_2pc(Job* pJob);
  static void vSpawnJobAndWait(Job* pJob);
  
  static bool bSpawnJobDedicated(Job* pJob);
  static eEC  eValidateSession(i8 iTrace, Session* pSession, eAC eApiCall);
  
  private:
  
  static bool bSpawnTheJob(Job* pJob);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
