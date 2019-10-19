/*============================================================================*/
/* Steps_Cast.cpp - cast step group                                           */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Cast(void)
{
  vAddStep("(int)"_Stz,           &Step::vCastInt);
  vAddStep("(ptr)"_Stz,           &Step::vCastPtr);
}

/*----------------------------------------------------------------------------*/
/* (int) -- cast ptr to int                                                   */
/*----------------------------------------------------------------------------*/
void Step::vCastInt(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vCastInt"_Stz);
  
  Object* pPtr;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtr) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  mpTask->m_pForth->m_pDataStack->vPushI8((i8)pPtr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* (ptr) -- cast int to ptr                                                   */
/*----------------------------------------------------------------------------*/
void Step::vCastPtr(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vCastPtr"_Stz);
  
  i8 iInt;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(iInt) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  mpTask->m_pForth->m_pDataStack->vPushPointer((Object*)iInt);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
