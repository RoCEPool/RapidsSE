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
  static bool bSpawnJob(Session* pSession, Job* pJob);
  static void vSpawnJobAndWait(Job* pJob, Session* pSession);
  static bool bSpawnJobDedicated(Job* pJob);
  static bool bSpawnOp(Session* pSession, Op* pOp);
  static eEC  eValidateSession(i8 iTrace, Session* pSession, eAC eApiCall);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
