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
  ENTRY("vActivityLogDump"_Stz);
  
  AM::vLogWriteCsv(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* log::mark -- write a log entry into the log                                */
/*----------------------------------------------------------------------------*/
void Step::vLogMark(void)
{
  ENTRY("vActivityLogMark"_Stz);
  
  s256 sMark;

  if ( ! m_pDataStack->bPopS256(sMark) )
    THROW_DSTACK();

  //i8 iJobId = m_pTask->m_pJob->m_iFocalId;
  JobLog::vNote(m_pTask->m_pJob,sMark);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* log::markEntry -- write an log entry for word entry                        */
/*----------------------------------------------------------------------------*/
void Step::vLogMarkEntry(void)
{
  ENTRY("vActivityLogMarkEntry"_Stz);
  
  s256 sWordNm;
  
  if ( ! m_pDataStack->bPopS256(sWordNm) )
    THROW_DSTACK();
  
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
  ENTRY("vActivityLogMarkExit"_Stz);
  
  s256 sWordNm;
  
  if ( ! m_pDataStack->bPopS256(sWordNm) )
    THROW_DSTACK();
  
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
  ENTRY("vActivityLogMarkPhase"_Stz);
  
  i8 i_Phase;
  if ( ! m_pDataStack->bPopI8(i_Phase) )
    THROW_DSTACK();
  
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
  ENTRY("vLogMarkSeed"_Stz);
  
  i8 iSeed;
  if ( ! m_pDataStack->bPopI8(iSeed) )
    THROW_DSTACK();
  
  s32 sWordNm;
  if ( ! m_pDataStack->bPopS32(sWordNm) )
    THROW_DSTACK();
  
  //ActLog::vAdd(m_pTask->m_pJob,(eAN)iSeed,sWordNm.szStz());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* log::markWord -- write a word mark into the action sequence                */
/*----------------------------------------------------------------------------*/
void Step::vLogMarkWord(void)
{
  ENTRY("vLogMarkWord"_Stz);
  
  i8 iAN;
  if ( ! m_pDataStack->bPopI8(iAN) )
    THROW_DSTACK();
  
  s32 sWordNm;
  if ( ! m_pDataStack->bPopS32(sWordNm) )
    THROW_DSTACK();
  
  //ActLog::vAdd(m_pTask->m_pJob,(eAN)iAN,sWordNm.szStz());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* log::jobSeed -- add a job seed entry into log                              */
/*----------------------------------------------------------------------------*/
void Step::vLogJobSeed(void)
{
  ENTRY("vLogJobSeed"_Stz);
  
  i8 i_Seed;
  if ( ! m_pDataStack->bPopI8(i_Seed) )
    THROW_DSTACK();
  
  Job* pJob = m_pTask->m_pJob;
  AM::vLogJob(SL_INF,pJob,JA_JOB_SEED,
              "%s"_Stz, //!format-LogJob
              aSS[i_Seed]);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
