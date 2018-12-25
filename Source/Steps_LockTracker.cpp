/*============================================================================*/
/* Steps_LockTracker.cpp - lock tracker step group                            */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_LockTracker(void)
{
  vAddStep("lockTracker::list"_Stz,  &Step::vLockTrackerList);
  vAddStep("lockTracker::reset"_Stz, &Step::vLockTrackerReset);
}

/*----------------------------------------------------------------------------*/
/* lockTracker::list -- list lock stats                                       */
/*----------------------------------------------------------------------------*/
void Step::vLockTrackerList(MP_Task mpTask)
{
ENTRY("vLockTrackerList"_Stz);

  File oFile;
  oFile.bOpen("locks.csv"_Stz, C_OPEN_WRITE);
  
  gpAttachMap->m_oLockTracker.vList(oFile);
  
  oFile.vClose();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* lockTracker::reset -- reset locks                                          */
/*----------------------------------------------------------------------------*/
void Step::vLockTrackerReset(MP_Task mpTask)
{
  ENTRY("vLockTrackerReset"_Stz);
  
  gpAttachMap->m_oLockTracker.vReset();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
