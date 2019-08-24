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
  
  vAddStep("ActLog::markSeed"_Stz, &Step::vActionSequenceMarkSeed);
  vAddStep("ActLog::markWord"_Stz, &Step::vActionSequenceMarkWord);
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
    
  if (mpTask->m_pJob->m_xReqTuple.m_bLogJob)
    JobLog::vNote(mpTask->m_pJob,
                          "%s %s %p"_Stz,
                          sWordNm.aStz(),
                          aCN[mpTask->pJob()->eClass()],
                          mpTask->pJob()
                          );
  
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
  
  if (mpTask->m_pJob->m_xReqTuple.m_bLogJob)
    JobLog::vNote(mpTask->m_pJob,
                          "%s %s %p"_Stz,
                          sWordNm.aStz(),
                          aCN[mpTask->pJob()->eClass()],
                          mpTask->pJob()
                          );

  EXIT();
}

////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* actionSequence::markSeed -- write a seed mark into the action sequence     */
/*----------------------------------------------------------------------------*/
void Step::vActionSequenceMarkSeed(MP_Task mpTask)
{
  ENTRY("vActionSequenceMarkSeed"_Stz);
  
  i8 iSeed;
  if ( ! mpTask->m_pDataStack->bPop(iSeed) )
    THROW_INVALID_DSTACK("actionSequence::markSeed"_Stz);
  
  s32 sWordNm;
  if ( ! mpTask->m_pDataStack->bPop(sWordNm) )
    THROW_INVALID_DSTACK("actionSequence::markSeed"_Stz);
  
  //ActLog::vAdd(mpTask->m_pJob,(eAN)iSeed,sWordNm.aStz());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* actionSequence::markWord -- write a word mark into the action sequence     */
/*----------------------------------------------------------------------------*/
void Step::vActionSequenceMarkWord(MP_Task mpTask)
{
  ENTRY("vActionSequenceMarkWord"_Stz);
  
  i8 iAN;
  if ( ! mpTask->m_pDataStack->bPop(iAN) )
    THROW_INVALID_DSTACK("actionSequence::markWord"_Stz);
  
  s32 sWordNm;
  if ( ! mpTask->m_pDataStack->bPop(sWordNm) )
    THROW_INVALID_DSTACK("actionSequence::markWord"_Stz);
  
  //ActLog::vAdd(mpTask->m_pJob,(eAN)iAN,sWordNm.aStz());
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
