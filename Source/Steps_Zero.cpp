/*============================================================================*/
/* Steps_Zero.cpp - zero step group                                           */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Zero(void)
{
  vAddStep("0<"_Stz,              &Step::vZeroLess);
  vAddStep("0<="_Stz,             &Step::vZeroLessEqual);
  vAddStep("0=="_Stz,             &Step::vZeroEqual);
  vAddStep("0!="_Stz,             &Step::vZeroNotEqual);
  vAddStep("0>="_Stz,             &Step::vZeroGreaterEqual);
  vAddStep("0>"_Stz,              &Step::vZeroGreater);
}

/*----------------------------------------------------------------------------*/
/* 0< -- test if top value is less than zero                                  */
/*----------------------------------------------------------------------------*/
void Step::vZeroLess(MP_Task mpTask)
{
  ENTRY("vZeroLess"_Stz);
  
  i8 iZero;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iZero) )
    THROW_INVALID_DSTACK("0<"_Stz);
  
  if (iZero < 0)
    mpTask->m_pDataStack->vPushBool(true);
  else
    mpTask->m_pDataStack->vPushBool(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 0<= -- test if top value is less than or equal to zero                     */
/*----------------------------------------------------------------------------*/
void Step::vZeroLessEqual(MP_Task mpTask)
{
  ENTRY("vZeroLessEqual"_Stz);
  
  i8 iZero;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iZero) )
    THROW_INVALID_DSTACK("0<="_Stz);
  
  if (iZero <= 0)
    mpTask->m_pDataStack->vPushBool(true);
  else
    mpTask->m_pDataStack->vPushBool(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 0== -- test if top value is equal to zero / nullptr                        */
/*----------------------------------------------------------------------------*/
void Step::vZeroEqual(MP_Task mpTask)
{
  ENTRY("vZeroEqual"_Stz);
  
  i8 iZero;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iZero) )
    THROW_INVALID_DSTACK("0=="_Stz);
  
  if (iZero == 0)
    mpTask->m_pDataStack->vPushBool(true);
  else
    mpTask->m_pDataStack->vPushBool(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 0!= -- test if top value is not equal to zero                              */
/*----------------------------------------------------------------------------*/
void Step::vZeroNotEqual(MP_Task mpTask)
{
  ENTRY("vZeroNotEqual"_Stz);
  
  i8 iZero;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iZero) )
    THROW_INVALID_DSTACK("0!="_Stz);
  
  if (iZero != 0)
    mpTask->m_pDataStack->vPushBool(true);
  else
    mpTask->m_pDataStack->vPushBool(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* zeroGreaterEqual -- test if top value is greater than or equal to zero     */
/*----------------------------------------------------------------------------*/
void Step::vZeroGreaterEqual(MP_Task mpTask)
{
  ENTRY("vZeroGreaterEqual"_Stz);
  
  i8 iZero;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iZero) )
    THROW_INVALID_DSTACK("0>="_Stz);
  
  if (iZero >= 0)
    mpTask->m_pDataStack->vPushBool(true);
  else
    mpTask->m_pDataStack->vPushBool(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 0> -- test if top value is greater than zero                              */
/*----------------------------------------------------------------------------*/
void Step::vZeroGreater(MP_Task mpTask)
{
  ENTRY("vZeroGreater"_Stz);
  
  i8 iZero;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iZero) )
    THROW_INVALID_DSTACK("0>"_Stz);
  
  if (iZero > 0)
    mpTask->m_pDataStack->vPushBool(true);
  else
    mpTask->m_pDataStack->vPushBool(false);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
