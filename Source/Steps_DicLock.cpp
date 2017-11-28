/*============================================================================*/
/* Steps_DicLock.cpp - dic lock step group                                    */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

// diclock: ////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_DicLock(void)
{
  vAddPrimitive("dicLock.dropWrite"_Stz,&vDicLockDropWrite, false);
  vAddPrimitive("dicLock.grabWrite"_Stz,&vDicLockGrabWrite, false);
}

/*----------------------------------------------------------------------------*/
/* diclLock.dropWrite -- drop dic lock for write                              */
/*----------------------------------------------------------------------------*/
void Task::vDicLockDropWrite(MP_Task mpTask)
{
  ENTRY("vDicLockDropWrite"_Stz);
  
  gpAttachMap->m_xDicLock.vDropWrite();
  
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicLock.grabWrite -- grab dic lock for write                               */
/*----------------------------------------------------------------------------*/
void Task::vDicLockGrabWrite(MP_Task mpTask)
{
  ENTRY("vDicLockGrabWrite"_Stz);
  
  gpAttachMap->m_xDicLock.vGrabWrite();
    
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
