/*============================================================================*/
/* Steps_SchLock.cpp - scheduling lock step group                             */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_SchLock(void)
{
  vAddStep("SchLock.dropWrite"_Stz,&Step::vSchLockDropWrite);
  vAddStep("SchLock.grabWrite"_Stz,&Step::vSchLockGrabWrite);
}

/*----------------------------------------------------------------------------*/
/* diclLock.dropWrite -- drop dic lock for write                              */
/*----------------------------------------------------------------------------*/
void Step::vSchLockDropWrite(MP_Task mpTask)
{
  ENTRY("vSchLockDropWrite"_Stz);
  
  gpAttachMap->m_okSchLock.vDropWrite();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* SchLock.grabWrite -- grab dic lock for write                               */
/*----------------------------------------------------------------------------*/
void Step::vSchLockGrabWrite(MP_Task mpTask)
{
  ENTRY("vSchLockGrabWrite"_Stz);
  
  gpAttachMap->m_okSchLock.vGrabWrite();
    
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
