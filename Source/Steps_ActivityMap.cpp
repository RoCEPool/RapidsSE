/*============================================================================*/
/* Steps_ActivityMap.cpp -- Activity Map step group                           */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_ActivityMap(void)
{
  vAddStep("activityMap::dump"_Stz,       &Step::vActivityMapDump);
  vAddStep("activityMap::mark"_Stz,       &Step::vActivityMapMark);
  vAddStep("activityMap::markEntry"_Stz,  &Step::vActivityMapMarkEntry);
  vAddStep("activityMap::markExit"_Stz,   &Step::vActivityMapMarkExit);
  
  vAddStep("ActionSequence::markSeed"_Stz, &Step::vActionSequenceMarkSeed);
  vAddStep("ActionSequence::markWord"_Stz, &Step::vActionSequenceMarkWord);
}

/*----------------------------------------------------------------------------*/
/* activityMap::dump -- dump the activity map                                 */
/*----------------------------------------------------------------------------*/
void Step::vActivityMapDump(MP_Task mpTask)
{
  ENTRY("vActivityMapDump"_Stz);
  
  ActivityMap::vWriteCsv(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* activityMap::mark -- write a mark into the activity map                    */
/*----------------------------------------------------------------------------*/
void Step::vActivityMapMark(MP_Task mpTask)
{
  ENTRY("vActivityMapMark"_Stz);
  
  s256 sMark;

  if ( ! mpTask->m_pDataStack->bPop(sMark) )
    THROW_INVALID_DSTACK("activityMap::mark"_Stz);

  i8 iJobId = mpTask->m_pJob->m_iJobId;
  ActivityMap::vMarkJob(mpTask->m_pJob,sMark);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* activityMap::markEntry -- write a mark for word entry                      */
/*----------------------------------------------------------------------------*/
void Step::vActivityMapMarkEntry(MP_Task mpTask)
{
  ENTRY("vActivityMapMarkEntry"_Stz);
  
  s256 sWordNm;
  
  if ( ! mpTask->m_pDataStack->bPop(sWordNm) )
    THROW_INVALID_DSTACK("activityMap::markEntry"_Stz);
    
  if (mpTask->m_pJob->m_bMarkActivity)
    ActivityMap::vMarkJob(mpTask->m_pJob,
                          "%s %s %p"_Stz,
                          sWordNm.aStz(),
                          aCN[mpTask->pJob()->eClass()],
                          mpTask->pJob()
                          );
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* activityMap::mark -- write a mark for word exit                            */
/*----------------------------------------------------------------------------*/
void Step::vActivityMapMarkExit(MP_Task mpTask)
{
  ENTRY("vActivityMapMarkExit"_Stz);
  
  s256 sWordNm;
  
  if ( ! mpTask->m_pDataStack->bPop(sWordNm) )
    THROW_INVALID_DSTACK("activityMap::markExit"_Stz);
  
  if (mpTask->m_pJob->m_bMarkActivity)
    ActivityMap::vMarkJob(mpTask->m_pJob,
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
