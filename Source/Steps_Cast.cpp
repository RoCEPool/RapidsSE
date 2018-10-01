/*============================================================================*/
/* Steps_Cast.cpp - cast step group                                           */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

// cast: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Cast(void)
{
  vAddStep("(int)"_Stz,           &Task::vCastInt,          false);
  vAddStep("(ptr)"_Stz,           &Task::vCastPtr,          false);
}

/*----------------------------------------------------------------------------*/
/* (int) -- cast ptr to int                                                   */
/*----------------------------------------------------------------------------*/
void Task::vCastInt(MP_Task mpTask)
{
  ENTRY("vCastInt"_Stz);
  
  Object* pPtr;
  if ( ! mpTask->m_pDataStack->bPop(pPtr) )
    THROW_INVALID_DSTACK("(int)"_Stz);
  
  mpTask->m_pDataStack->vPushInt((i8)pPtr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* (ptr) -- cast int to ptr                                                   */
/*----------------------------------------------------------------------------*/
void Task::vCastPtr(MP_Task mpTask)
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
