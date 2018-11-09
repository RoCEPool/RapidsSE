/*============================================================================*/
/* Steps_LockTracker.cpp - lock tracker step group                            */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

// steps: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_LockTracker(void)
{
  vAddStep("lockTracker::list"_Stz,  &Task::vLockTrackerList);
  vAddStep("lockTracker::reset"_Stz, &Task::vLockTrackerReset);
}

/*----------------------------------------------------------------------------*/
/* lockTracker::list -- list lock stats                                       */
/*----------------------------------------------------------------------------*/
void Task::vLockTrackerList(MP_Task mpTask)
{
ENTRY("vLockTrackerList"_Stz);

  File xFile;
  xFile.bOpen("locks.csv"_Stz, C_OPEN_WRITE);
  
  gpAttachMap->m_xLockTracker.vList(xFile);
  
  xFile.vClose();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* lockTracker::reset -- reset locks                                          */
/*----------------------------------------------------------------------------*/
void Task::vLockTrackerReset(MP_Task mpTask)
{
  ENTRY("vLockTrackerReset"_Stz);
  
  gpAttachMap->m_xLockTracker.vReset();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
