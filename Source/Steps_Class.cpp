/*============================================================================*/
/* Steps_Class.cpp - class steps group                                        */
/*============================================================================*/

#include "Steps.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Class(void)
{
  vAddStep("class::name"_Stz,      &Step::vClassName);
}

/*----------------------------------------------------------------------------*/
/* class::name -- translate class no to name                                  */
/*----------------------------------------------------------------------------*/
void Step::vClassName(MP_Task mpTask)
{
  ENTRY("vClassName"_Stz);
  
  i8 iClassNo;
  if ( ! mpTask->m_pDataStack->bPop(iClassNo) )
    THROW_INVALID_DSTACK("class::name"_Stz);
  
  s32 sClassName;
  
  sClassName = aCN[iClassNo];
  
  mpTask->m_pDataStack->vPushS32(sClassName);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
