/*============================================================================*/
/* Steps_Cast.cpp - cast step group                                           */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

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
void Step::vCastInt(MP_Task mpTask)
{
  ENTRY("vCastInt"_Stz);
  
  Object* pPtr;
  if ( ! mpTask->m_pDataStack->bPop(pPtr) )
    THROW_INVALID_DSTACK("(int)"_Stz);
  
  mpTask->m_pDataStack->vPushI8((i8)pPtr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* (ptr) -- cast int to ptr                                                   */
/*----------------------------------------------------------------------------*/
void Step::vCastPtr(MP_Task mpTask)
{
  ENTRY("vCastPtr"_Stz);
  
  i8 iInt;
  if ( ! mpTask->m_pDataStack->bPop(iInt) )
    THROW_INVALID_DSTACK("(ptr)"_Stz);
  
  mpTask->m_pDataStack->vPushObject((Object*)iInt);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
