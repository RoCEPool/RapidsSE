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
void Step::vLogDump(MP_Task mpTask)
{
  ENTRY("vActivityLogDump"_Stz);
  
  AM::vLogWriteCsv(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* log::mark -- write a log entry into the log                                */
/*----------------------------------------------------------------------------*/
void Step::vLogMark(MP_Task mpTask)
{
  ENTRY("vActivityLogMark"_Stz);
  
  s256 sMark;

  if ( ! mpTask->m_pDataStack->bPop(sMark) )
    THROW_INVALID_DSTACK("log::mark"_Stz);

  //i8 iJobId = mpTask->m_pJob->m_iUniqueId;
  JobLog::vNote(mpTask->m_pJob,sMark);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* log::markEntry -- write an log entry for word entry                        */
/*----------------------------------------------------------------------------*/
void Step::vLogMarkEntry(MP_Task mpTask)
{
  ENTRY("vActivityLogMarkEntry"_Stz);
  
  s256 sWordNm;
  
  if ( ! mpTask->m_pDataStack->bPop(sWordNm) )
    THROW_INVALID_DSTACK("log::markEntry"_Stz);
  
  Job* pJob = mpTask->m_pJob;
  AM::vLogJob(SL_INF,pJob,JA_JOB_ENTER,
              "%s"_Stz, //!format-LogJob
              sWordNm.aStz());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* log::mark -- write an log entry for word exit                              */
/*----------------------------------------------------------------------------*/
void Step::vLogMarkExit(MP_Task mpTask)
{
  ENTRY("vActivityLogMarkExit"_Stz);
  
  s256 sWordNm;
  
  if ( ! mpTask->m_pDataStack->bPop(sWordNm) )
    THROW_INVALID_DSTACK("log::markExit"_Stz);
  
  Job* pJob = mpTask->m_pJob;

  AM::vLogJob(SL_INF,pJob,JA_JOB_EXIT,
              "%s"_Stz, //!format-LogJob
              sWordNm.aStz());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* log::mark -- write an log entry for word exit                              */
/*----------------------------------------------------------------------------*/
void Step::vLogMarkPhase(MP_Task mpTask)
{
  ENTRY("vActivityLogMarkPhase"_Stz);
  
  i8 i_Phase;
  if ( ! mpTask->m_pDataStack->bPop(i_Phase) )
    THROW_INVALID_DSTACK("log::markPhase"_Stz);
  
  Job* pJob = mpTask->m_pJob;
  
  AM::vLogJob(SL_INF,pJob,JA_JOB_PHASE,
              "Phase : %s for %s %p"_Stz,
              aJP[i_Phase],
              aCN[pJob->eClass()],
              pJob
              );
  
  EXIT();
}

////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* log::markSeed -- write a seed mark into the action sequence                */
/*----------------------------------------------------------------------------*/
void Step::vLogMarkSeed(MP_Task mpTask)
{
  ENTRY("vLogMarkSeed"_Stz);
  
  i8 iSeed;
  if ( ! mpTask->m_pDataStack->bPop(iSeed) )
    THROW_INVALID_DSTACK("log::markSeed"_Stz);
  
  s32 sWordNm;
  if ( ! mpTask->m_pDataStack->bPop(sWordNm) )
    THROW_INVALID_DSTACK("log::markSeed"_Stz);
  
  //ActLog::vAdd(mpTask->m_pJob,(eAN)iSeed,sWordNm.aStz());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* log::markWord -- write a word mark into the action sequence                */
/*----------------------------------------------------------------------------*/
void Step::vLogMarkWord(MP_Task mpTask)
{
  ENTRY("vLogMarkWord"_Stz);
  
  i8 iAN;
  if ( ! mpTask->m_pDataStack->bPop(iAN) )
    THROW_INVALID_DSTACK("log::markWord"_Stz);
  
  s32 sWordNm;
  if ( ! mpTask->m_pDataStack->bPop(sWordNm) )
    THROW_INVALID_DSTACK("log::markWord"_Stz);
  
  //ActLog::vAdd(mpTask->m_pJob,(eAN)iAN,sWordNm.aStz());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* log::jobSeed -- add a job seed entry into log                              */
/*----------------------------------------------------------------------------*/
void Step::vLogJobSeed(MP_Task mpTask)
{
  ENTRY("vLogJobSeed"_Stz);
  
  i8 i_Seed;
  if ( ! mpTask->m_pDataStack->bPop(i_Seed) )
    THROW_INVALID_DSTACK("log::JobSeed"_Stz);
  
  Job* pJob = mpTask->m_pJob;
  AM::vLogJob(SL_INF,pJob,JA_JOB_SEED,
              "%s"_Stz, //!format-LogJob
              aSS[i_Seed]);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
