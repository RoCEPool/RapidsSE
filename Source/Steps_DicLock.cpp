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
void Step::vAddSteps_DicLock(void)
{
  vAddStep("dicLock.dropWrite"_Stz,&Step::vDicLockDropWrite);
  vAddStep("dicLock.grabWrite"_Stz,&Step::vDicLockGrabWrite);
}

/*----------------------------------------------------------------------------*/
/* diclLock.dropWrite -- drop dic lock for write                              */
/*----------------------------------------------------------------------------*/
void Step::vDicLockDropWrite(MP_Task mpTask)
{
  ENTRY("vDicLockDropWrite"_Stz);
  
  gpAttachMap->m_xDicLock.vDropWrite();
  
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicLock.grabWrite -- grab dic lock for write                               */
/*----------------------------------------------------------------------------*/
void Step::vDicLockGrabWrite(MP_Task mpTask)
{
  ENTRY("vDicLockGrabWrite"_Stz);
  
  gpAttachMap->m_xDicLock.vGrabWrite();
    
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
