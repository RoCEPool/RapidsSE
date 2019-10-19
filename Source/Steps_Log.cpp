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
void Step::vLogDump(Forth* pForth)
{
  ENTRY("vActivityLogDump"_Stz);
  
  AM::vLogWriteCsv(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* log::mark -- write a log entry into the log                                */
/*----------------------------------------------------------------------------*/
void Step::vLogMark(Forth* pForth)
{
  ENTRY("vActivityLogMark"_Stz);
  
  s256 sMark;

  if ( ! pForth->m_pDataStack->bPop(sMark) )
    THROW_DSTACK();

  //i8 iJobId = pForth->m_pTask->m_pJob->m_iUniqueId;
  JobLog::vNote(pForth->m_pTask->m_pJob,sMark);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* log::markEntry -- write an log entry for word entry                        */
/*----------------------------------------------------------------------------*/
void Step::vLogMarkEntry(Forth* pForth)
{
  ENTRY("vActivityLogMarkEntry"_Stz);
  
  s256 sWordNm;
  
  if ( ! pForth->m_pDataStack->bPop(sWordNm) )
    THROW_DSTACK();
  
  Job* pJob = pForth->m_pTask->m_pJob;
  AM::vLogJob(SL_INF,pJob,JA_JOB_ENTER,
              "%s"_Stz, //!format-LogJob
              sWordNm.szStz());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* log::mark -- write an log entry for word exit                              */
/*----------------------------------------------------------------------------*/
void Step::vLogMarkExit(Forth* pForth)
{
  ENTRY("vActivityLogMarkExit"_Stz);
  
  s256 sWordNm;
  
  if ( ! pForth->m_pDataStack->bPop(sWordNm) )
    THROW_DSTACK();
  
  Job* pJob = pForth->m_pTask->m_pJob;

  AM::vLogJob(SL_INF,pJob,JA_JOB_EXIT,
              "%s"_Stz, //!format-LogJob
              sWordNm.szStz());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* log::mark -- write an log entry for word exit                              */
/*----------------------------------------------------------------------------*/
void Step::vLogMarkPhase(Forth* pForth)
{
  ENTRY("vActivityLogMarkPhase"_Stz);
  
  i8 i_Phase;
  if ( ! pForth->m_pDataStack->bPop(i_Phase) )
    THROW_DSTACK();
  
  Job* pJob = pForth->m_pTask->m_pJob;
  
  AM::vLogJob(SL_INF,pJob,JA_JOB_PHASE,
              "%s"_Stz,
              aJP[i_Phase]);
  
  EXIT();
}

////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* log::markSeed -- write a seed mark into the action sequence                */
/*----------------------------------------------------------------------------*/
void Step::vLogMarkSeed(Forth* pForth)
{
  ENTRY("vLogMarkSeed"_Stz);
  
  i8 iSeed;
  if ( ! pForth->m_pDataStack->bPop(iSeed) )
    THROW_DSTACK();
  
  s32 sWordNm;
  if ( ! pForth->m_pDataStack->bPop(sWordNm) )
    THROW_DSTACK();
  
  //ActLog::vAdd(pForth->m_pTask->m_pJob,(eAN)iSeed,sWordNm.szStz());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* log::markWord -- write a word mark into the action sequence                */
/*----------------------------------------------------------------------------*/
void Step::vLogMarkWord(Forth* pForth)
{
  ENTRY("vLogMarkWord"_Stz);
  
  i8 iAN;
  if ( ! pForth->m_pDataStack->bPop(iAN) )
    THROW_DSTACK();
  
  s32 sWordNm;
  if ( ! pForth->m_pDataStack->bPop(sWordNm) )
    THROW_DSTACK();
  
  //ActLog::vAdd(pForth->m_pTask->m_pJob,(eAN)iAN,sWordNm.szStz());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* log::jobSeed -- add a job seed entry into log                              */
/*----------------------------------------------------------------------------*/
void Step::vLogJobSeed(Forth* pForth)
{
  ENTRY("vLogJobSeed"_Stz);
  
  i8 i_Seed;
  if ( ! pForth->m_pDataStack->bPop(i_Seed) )
    THROW_DSTACK();
  
  Job* pJob = pForth->m_pTask->m_pJob;
  AM::vLogJob(SL_INF,pJob,JA_JOB_SEED,
              "%s"_Stz, //!format-LogJob
              aSS[i_Seed]);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
