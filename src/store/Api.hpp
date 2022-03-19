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
  // parsimonious API
  
  //static Op* pGetOperator(Stz* szOpNm);
  static Operand* pGetOperand(eZK eOperand);

  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // external API functions
  
  static Session* pAllocSession(eSK eSession);
  static void vFreeSession(Session* pSession);

  static void vDeleteJob(Session* pSession);
  static void vExamineCommand(Session* pSession);
  static void vExecuteJob(Session* pSession);
  static void vExtractCmdPaths(Session*  pSession);
  static bool bExtractCmdTags(Session*  pSession);
  static eEC  ePrepareJob(Session*  pSession, TokenVec* pDemoLiterals = nullptr);

  static void vSubmitJob(Session* pSession);
  static void vWaitJob(Session* pSession);

  static void vPushOperand(Session* pSession, Operand* pOperand);
 
  static void vReapAllJobs(bool bForce = false);
  static void vReapJobsOnce(bool bForce = false);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  static void vScheduleNextPhase(Job* pJob, Task* pActiviating2PcTask);
  
  // external
  static bool bSpawnTask_session(Job* pJob);
  
  // internal
  
  static void vCallBootSystem(void);
  static void vCallStartSystem(void);

  static void vCallImmediate(Task* pImmediateTask);
  static void vDeleteImmediate(Task** ppTask);

  static void vSpawnTask_job(Job* pJob);
  static void vSpawnTask_op(Op* pop);
  static void vSpawnTaskDedicated_job(Job* pJob);
  static void vSpawnTaskDedicated_op(Op* pOp);

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

  
  static void vSetTaskInxPtr(Task* pTask, Stz* szEntryPt);

  private:
  
  static void vCreateTaskForJob(Job* pJob);
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
