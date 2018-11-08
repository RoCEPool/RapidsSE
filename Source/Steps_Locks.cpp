/*============================================================================*/
/* Steps_LockTrack.cpp - lock tracker step group                              */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

// steps: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Locks(void)
{
  vAddStep("locks::list"_Stz,  &Task::vLocksList);
  vAddStep("locks::reset"_Stz, &Task::vLocksReset);
}

/*----------------------------------------------------------------------------*/
/* locks::list -- list lock stats                                             */
/*----------------------------------------------------------------------------*/
void Task::vLocksList(MP_Task mpTask)
{
ENTRY("vLocksList"_Stz);

  File xFile;
  xFile.bOpen("locks.csv"_Stz, C_OPEN_WRITE);
  
  gpAttachMap->m_xLockTracker.vList(xFile);
  
  xFile.vClose();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* locks::reset -- reset locks                                                 */
/*----------------------------------------------------------------------------*/
void Task::vLocksReset(MP_Task mpTask)
{
  ENTRY("vLocksReset"_Stz);
  
  gpAttachMap->m_xLockTracker.vReset();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
