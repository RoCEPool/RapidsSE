/*============================================================================*/
/* Steps_DicLock.cpp - dictionary lock step group                             */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_DicLock(void)
{
  vAddStep("DicLock.dropWrite"_Stz,&Step::vDicLockDropWrite);
  vAddStep("DicLock.grabWrite"_Stz,&Step::vDicLockGrabWrite);
}

/*----------------------------------------------------------------------------*/
/* diclLock.dropWrite -- drop dic lock for write                              */
/*----------------------------------------------------------------------------*/
void Step::vDicLockDropWrite(MP_Task mpTask)
{
  ENTRY("vDicLockDropWrite"_Stz);
  
  AM::pDicLock()->vDropWrite();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* DicLock.grabWrite -- grab dic lock for write                               */
/*----------------------------------------------------------------------------*/
void Step::vDicLockGrabWrite(MP_Task mpTask)
{
  ENTRY("vDicLockGrabWrite"_Stz);
  
  AM::pDicLock()->vGrabWrite();
    
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
