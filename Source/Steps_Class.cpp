/*============================================================================*/
/* Steps_Class.cpp - class steps group                                        */
/*============================================================================*/

#include "Steps.h"

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
void Step::vClassName(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vClassName"_Stz);
  
  i8 iClassNo;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(iClassNo) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK);
  
  s32 sClassName;
  
  sClassName = aCN[iClassNo];
  
  mpTask->m_pForth->m_pDataStack->vPushS32(sClassName);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
