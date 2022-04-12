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

  static void vExecuteJob(Session* pSession);
  static void vExtractCmdPaths(Session*  pSession, TokenVec* pCmdTknVec);
  static bool bExtractCmdTags(Session*  pSession, TokenVec* pCmdTknVec);
  static void vForkJob(Session* pSession);
  static eEC  ePrepareJob(Session*  pSession, TokenVec* pCmdTknVec);
  static void vReapableJob(Session* pSession);

  static void vSubmitJob(Session* pSession);
  static void vWaitJob(Session* pSession);
 
  static void vReapAllJobs(bool bForce = false);
  static void vReapJobsOnce(bool bForce = false);

  static bool bExecuteCommand(Stz* szCmd, Stz* szWithTagStr, Stz* szTagPath,
                              eEC& reError, i8& iJobNo);

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

  static void vMutexLock(Session* pSession, Job* pJob);
  static void vMutexUnlock(Session* pSession, Job* pJob);
  static void vMutexWaitCond(Session* pSession, Job* pJob);

  static bool bReapJob(void);


  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
