/*============================================================================*/
/* Steps_ActivityLog.cpp -- Activity Log step group                           */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Log(void)
{
  vAddStep("log::dump"_Stz,       &Step::vLogDump);
  vAddStep("log::mark"_Stz,       &Step::vLogMark);
  vAddStep("log::markEntry"_Stz,  &Step::vLogMarkEntry);
  vAddStep("log::markExit"_Stz,   &Step::vLogMarkExit);
  vAddStep("log::markPhase"_Stz,  &Step::vLogMarkPhase);

  vAddStep("Log::markSeed"_Stz,   &Step::vLogMarkSeed);
  vAddStep("Log::markWord"_Stz,   &Step::vLogMarkWord);
  
  vAddStep("Log::jobSeed"_Stz,    &Step::vLogJobSeed);
}

/*----------------------------------------------------------------------------*/
/* log::dump -- dump the log                                                  */
/*----------------------------------------------------------------------------*/
void Step::vLogDump(void)
{
  ENTRY();
  
  AM::vLogWriteCsv(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* log::mark -- write a log entry into the log                                */
/*----------------------------------------------------------------------------*/
void Step::vLogMark(void)
{
  ENTRY();
  
  s256 sMark;

  m_pDataStack->vPopStr(sMark,__func__);

  //i8 iJobId = m_pTask->m_pJob->m_iFocalId;
  JobLog::vNote(m_pTask->m_pJob,sMark);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* log::markEntry -- write an log entry for word entry                        */
/*----------------------------------------------------------------------------*/
void Step::vLogMarkEntry(void)
{
  ENTRY();
  
  s256 sWordNm;
  
  m_pDataStack->vPopStr(sWordNm,__func__);
  
  Job* pJob = m_pTask->m_pJob;
  AM::vLogJob(SL_INF,pJob,JA_JOB_ENTER,
              "%s"_Stz, //!format-LogJob
              sWordNm.szStz());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* log::mark -- write an log entry for word exit                              */
/*----------------------------------------------------------------------------*/
void Step::vLogMarkExit(void)
{
  ENTRY();
  
  s256 sWordNm;
  
  m_pDataStack->vPopStr(sWordNm,__func__);
  
  Job* pJob = m_pTask->m_pJob;

  AM::vLogJob(SL_INF,pJob,JA_JOB_EXIT,
              "%s"_Stz, //!format-LogJob
              sWordNm.szStz());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* log::mark -- write an log entry for word exit                              */
/*----------------------------------------------------------------------------*/
void Step::vLogMarkPhase(void)
{
  ENTRY();
  
  i8 i_Phase = m_pDataStack->iPopInt(__func__);
  
  Job* pJob = m_pTask->m_pJob;
  
  AM::vLogJob(SL_INF,pJob,JA_JOB_PHASE,
              "%s"_Stz,
              aJP[i_Phase]);
  
  EXIT();
}

////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* log::markSeed -- write a seed mark into the action sequence                */
/*----------------------------------------------------------------------------*/
void Step::vLogMarkSeed(void)
{
  ENTRY();
  
  i8 iSeed = m_pDataStack->iPopInt(__func__);
  
  s32 sWordNm;
  m_pDataStack->vPopStr(sWordNm,__func__);
  
  //ActLog::vAdd(m_pTask->m_pJob,(eAN)iSeed,sWordNm.szStz());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* log::markWord -- write a word mark into the action sequence                */
/*----------------------------------------------------------------------------*/
void Step::vLogMarkWord(void)
{
  ENTRY();
  
  i8 iAN = m_pDataStack->iPopInt(__func__);
  
  s32 sWordNm;
  m_pDataStack->vPopStr(sWordNm,__func__);
  
  //ActLog::vAdd(m_pTask->m_pJob,(eAN)iAN,sWordNm.szStz());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* log::jobSeed -- add a job seed entry into log                              */
/*----------------------------------------------------------------------------*/
void Step::vLogJobSeed(void)
{
  ENTRY();
  
  i8 i_Seed = m_pDataStack->iPopInt(__func__);
  
  Job* pJob = m_pTask->m_pJob;
  AM::vLogJob(SL_INF,pJob,JA_JOB_SEED,
              "%s"_Stz, //!format-LogJob
              aSS[i_Seed]);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
