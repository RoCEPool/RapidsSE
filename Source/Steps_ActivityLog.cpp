/*============================================================================*/
/* Steps_ActivityLog.cpp -- Activity Log step group                           */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_ActivityLog(void)
{
  vAddStep("activityLog::dump"_Stz,       &Step::vActivityLogDump);
  vAddStep("activityLog::mark"_Stz,       &Step::vActivityLogMark);
  vAddStep("activityLog::markEntry"_Stz,  &Step::vActivityLogMarkEntry);
  vAddStep("activityLog::markExit"_Stz,   &Step::vActivityLogMarkExit);
  
  vAddStep("ActionSequence::markSeed"_Stz, &Step::vActionSequenceMarkSeed);
  vAddStep("ActionSequence::markWord"_Stz, &Step::vActionSequenceMarkWord);
}

/*----------------------------------------------------------------------------*/
/* activityLog::dump -- dump the activity map                                 */
/*----------------------------------------------------------------------------*/
void Step::vActivityLogDump(MP_Task mpTask)
{
  ENTRY("vActivityLogDump"_Stz);
  
  ActivityLog::vWriteCsv(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* activityLog::mark -- write a mark into the activity map                    */
/*----------------------------------------------------------------------------*/
void Step::vActivityLogMark(MP_Task mpTask)
{
  ENTRY("vActivityLogMark"_Stz);
  
  s256 sMark;

  if ( ! mpTask->m_pDataStack->bPop(sMark) )
    THROW_INVALID_DSTACK("activityLog::mark"_Stz);

  //i8 iJobId = mpTask->m_pJob->m_iJobId;
  ActivityLog::vMarkJob(mpTask->m_pJob,sMark);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* activityLog::markEntry -- write a mark for word entry                      */
/*----------------------------------------------------------------------------*/
void Step::vActivityLogMarkEntry(MP_Task mpTask)
{
  ENTRY("vActivityLogMarkEntry"_Stz);
  
  s256 sWordNm;
  
  if ( ! mpTask->m_pDataStack->bPop(sWordNm) )
    THROW_INVALID_DSTACK("activityLog::markEntry"_Stz);
    
  if (mpTask->m_pJob->m_bMarkActivity)
    ActivityLog::vMarkJob(mpTask->m_pJob,
                          "%s %s %p"_Stz,
                          sWordNm.aStz(),
                          aCN[mpTask->pJob()->eClass()],
                          mpTask->pJob()
                          );
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* activityLog::mark -- write a mark for word exit                            */
/*----------------------------------------------------------------------------*/
void Step::vActivityLogMarkExit(MP_Task mpTask)
{
  ENTRY("vActivityLogMarkExit"_Stz);
  
  s256 sWordNm;
  
  if ( ! mpTask->m_pDataStack->bPop(sWordNm) )
    THROW_INVALID_DSTACK("activityLog::markExit"_Stz);
  
  if (mpTask->m_pJob->m_bMarkActivity)
    ActivityLog::vMarkJob(mpTask->m_pJob,
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
  
  ActionSequence::vAdd(mpTask->m_pJob,(eAN)iSeed,sWordNm.aStz());
  
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
  
  ActionSequence::vAdd(mpTask->m_pJob,(eAN)iAN,sWordNm.aStz());
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
