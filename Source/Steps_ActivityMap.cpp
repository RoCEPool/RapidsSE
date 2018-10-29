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
  vAddStep("activityMap::dump"_Stz,   &Task::vActivityMapDump);
  vAddStep("activityMap::mark"_Stz,   &Task::vActivityMapMark);
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
