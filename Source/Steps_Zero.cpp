/*============================================================================*/
/* Steps_Zero.cpp - zero step group                                           */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

// zero: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddSteps_Zero(void)
{
  vAddStep("0<"_Stz,              &Task::vZeroLess,         false);
  vAddStep("0<="_Stz,             &Task::vZeroLessEqual,    false);
  vAddStep("0=="_Stz,             &Task::vZeroEqual,        false);
  vAddStep("0!="_Stz,             &Task::vZeroNotEqual,     false);
  vAddStep("0>="_Stz,             &Task::vZeroGreaterEqual, false);
  vAddStep("0>"_Stz,              &Task::vZeroGreater,      false);
}

/*----------------------------------------------------------------------------*/
/* 0< -- test if top value is less than zero                                  */
/*----------------------------------------------------------------------------*/
void Task::vZeroLess(MP_Task mpTask)
{
  ENTRY("vZeroLess"_Stz);
  
  i8 iZero;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iZero) )
    THROW_INVALID_DSTACK("0<"_Stz);
  
  if (iZero < 0)
    mpTask->m_pDataStack->vPushInt((i8)1);
  else
    mpTask->m_pDataStack->vPushInt((i8)0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 0<= -- test if top value is less than or equal to zero                     */
/*----------------------------------------------------------------------------*/
void Task::vZeroLessEqual(MP_Task mpTask)
{
  ENTRY("vZeroLessEqual"_Stz);
  
  i8 iZero;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iZero) )
    THROW_INVALID_DSTACK("0<="_Stz);
  
  if (iZero <= 0)
    mpTask->m_pDataStack->vPushInt((i8)1);
  else
    mpTask->m_pDataStack->vPushInt((i8)0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 0== -- test if top value is equal to zero / nullptr                        */
/*----------------------------------------------------------------------------*/
void Task::vZeroEqual(MP_Task mpTask)
{
  ENTRY("vZeroEqual"_Stz);
  
  i8 iZero;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iZero) )
    THROW_INVALID_DSTACK("0=="_Stz);
  
  if (iZero == 0)
    mpTask->m_pDataStack->vPushInt((i8)1);
  else
    mpTask->m_pDataStack->vPushInt((i8)0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 0!= -- test if top value is not equal to zero                              */
/*----------------------------------------------------------------------------*/
void Task::vZeroNotEqual(MP_Task mpTask)
{
  ENTRY("vZeroNotEqual"_Stz);
  
  i8 iZero;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iZero) )
    THROW_INVALID_DSTACK("0!="_Stz);
  
  if (iZero != 0)
    mpTask->m_pDataStack->vPushInt((i8)1);
  else
    mpTask->m_pDataStack->vPushInt((i8)0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* zeroGreaterEqual -- test if top value is greater than or equal to zero     */
/*----------------------------------------------------------------------------*/
void Task::vZeroGreaterEqual(MP_Task mpTask)
{
  ENTRY("vZeroGreaterEqual"_Stz);
  
  i8 iZero;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iZero) )
    THROW_INVALID_DSTACK("0>="_Stz);
  
  if (iZero >= 0)
    mpTask->m_pDataStack->vPushInt((i8)1);
  else
    mpTask->m_pDataStack->vPushInt((i8)0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 0> -- test if top value is greater than zero                              */
/*----------------------------------------------------------------------------*/
void Task::vZeroGreater(MP_Task mpTask)
{
  ENTRY("vZeroGreater"_Stz);
  
  i8 iZero;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iZero) )
    THROW_INVALID_DSTACK("0>"_Stz);
  
  if (iZero > 0)
    mpTask->m_pDataStack->vPushInt((i8)1);
  else
    mpTask->m_pDataStack->vPushInt((i8)0);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
