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
}

/*----------------------------------------------------------------------------*/
/* activityMap::dump -- dump the activity map                                 */
/*----------------------------------------------------------------------------*/
void Step::vActivityMapDump(MP_Task mpTask)
{
  ENTRY("vActivityMapDump"_Stz);
  
  ActivityMap::vWriteCsv();
  
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
  ActivityMap::vMark(iJobId,sMark);
  
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
  
  if (mpTask->m_pJob->eClass() == CN_JOB_EXPORT)
    int iDebug = 1;
  
  i8 iJobId = mpTask->m_pJob->m_iJobId;
  if (mpTask->m_pJob->m_bMark)
    ActivityMap::vMark(iJobId,sWordNm);
  
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
  
  i8 iJobId = mpTask->m_pJob->m_iJobId;
  if (mpTask->m_pJob->m_bMark)
    ActivityMap::vMark(iJobId,sWordNm);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
