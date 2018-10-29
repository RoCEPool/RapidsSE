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
  vAddStep("activityMap::dump"_Stz,       &Task::vActivityMapDump);
  vAddStep("activityMap::mark"_Stz,       &Task::vActivityMapMark);
  vAddStep("activityMap::markEntry"_Stz,  &Task::vActivityMapMarkEntry);
  vAddStep("activityMap::markExit"_Stz,   &Task::vActivityMapMarkExit);
}

/*----------------------------------------------------------------------------*/
/* activityMap::dump -- dump the activity map                                 */
/*----------------------------------------------------------------------------*/
void Task::vActivityMapDump(MP_Task mpTask)
{
  ENTRY("vActivityMapDump"_Stz);
  
  ActivityMap::vDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* activityMap::mark -- write a mark into the activity map                    */
/*----------------------------------------------------------------------------*/
void Task::vActivityMapMark(MP_Task mpTask)
{
  ENTRY("vActivityMapMark"_Stz);
  
  s256 sMark;

  if ( ! mpTask->m_pDataStack->bPop(sMark) )
    THROW_INVALID_DSTACK("activityMap::mark"_Stz);

  ActivityMap::vMark(sMark);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* activityMap::markEntry -- write a mark for word entry                      */
/*----------------------------------------------------------------------------*/
void Task::vActivityMapMarkEntry(MP_Task mpTask)
{
  ENTRY("vActivityMapMarkEntry"_Stz);
  
  s256 sWordNm;
  
  if ( ! mpTask->m_pDataStack->bPop(sWordNm) )
    THROW_INVALID_DSTACK("activityMap::markEntry"_Stz);
  
  if (mpTask->m_pJob->m_bMark)
    ActivityMap::vMark(sWordNm);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* activityMap::mark -- write a mark for word exit                            */
/*----------------------------------------------------------------------------*/
void Task::vActivityMapMarkExit(MP_Task mpTask)
{
  ENTRY("vActivityMapMarkExit"_Stz);
  
  s256 sWordNm;
  
  if ( ! mpTask->m_pDataStack->bPop(sWordNm) )
    THROW_INVALID_DSTACK("activityMap::markExit"_Stz);
  
  if (mpTask->m_pJob->m_bMark)
    ActivityMap::vMark(sWordNm);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
