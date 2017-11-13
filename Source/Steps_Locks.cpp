/*============================================================================*/
/* Steps_LockTrack.cpp - lock tracker step group                              */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

extern Locks           gxLocks;

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
  
  gxLocks.vList(xFile);
  
  xFile.vClose();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* locks::reset -- reset locks                                                 */
/*----------------------------------------------------------------------------*/
void Task::vLocksReset(MP_Task mpTask)
{
  ENTRY("vLocksReset"_Stz);
  
  gxLocks.vReset();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
