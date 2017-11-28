/*============================================================================*/
/* Steps_LockTrack.cpp - lock tracker step group                              */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;
extern NP_Task         gnpBootTask;

// steps: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Locks(void)
{
  vAddPrimitive("locks::list"_Stz,  &vLocksList,  false);
  vAddPrimitive("locks::reset"_Stz, &vLocksReset, false);
}

/*----------------------------------------------------------------------------*/
/* locks::list -- list lock stats                                             */
/*----------------------------------------------------------------------------*/
void Task::vLocksList(MP_Task mpTask)
{
ENTRY("vLocksList"_Stz);

  File xFile;
  xFile.bOpen("locks.csv"_Stz, C_OPEN_WRITE);
  
  gpAttachMap->m_xLocks.vList(xFile);
  
  xFile.vClose();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* locks::reset -- reset locks                                                 */
/*----------------------------------------------------------------------------*/
void Task::vLocksReset(MP_Task mpTask)
{
  ENTRY("vLocksReset"_Stz);
  
  gpAttachMap->m_xLocks.vReset();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
