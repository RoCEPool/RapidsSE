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
  if ( ! pForth->m_pDataStack->bPop(pPtr) )
    THROW_DSTACK();
  
  pForth->m_pDataStack->vPushI8((i8)pPtr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* (ptr) -- cast int to ptr                                                   */
/*----------------------------------------------------------------------------*/
void Step::vCastPtr(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vCastPtr"_Stz);
  
  i8 iInt;
  if ( ! pForth->m_pDataStack->bPop(iInt) )
    THROW_DSTACK();
  
  pForth->m_pDataStack->vPushPointer((Object*)iInt);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
