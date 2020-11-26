/*============================================================================*/
/* Steps_Zero.cpp - zero step group                                           */
/*============================================================================*/

#include "Steps.h"

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
void Step::vZeroLess(void)
{
  ENTRY();
  
  i8 iZero = m_pDataStack->iPopAsInt(__func__);
  
  if (iZero < 0)
    m_pDataStack->vPushInt(true);
  else
    m_pDataStack->vPushInt(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 0<= -- test if top value is less than or equal to zero                     */
/*----------------------------------------------------------------------------*/
void Step::vZeroLessEqual(void)
{
  ENTRY();
  
  i8 iZero = m_pDataStack->iPopAsInt(__func__);
  
  if (iZero <= 0)
    m_pDataStack->vPushInt(true);
  else
    m_pDataStack->vPushInt(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 0== -- test if top value is equal to zero / nullptr                        */
/*----------------------------------------------------------------------------*/
void Step::vZeroEqual(void)
{
  ENTRY();
  
  i8 iZero = m_pDataStack->iPopAsInt(__func__);
  
  if (iZero == 0)
    m_pDataStack->vPushInt(true);
  else
    m_pDataStack->vPushInt(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 0!= -- test if top value is not equal to zero                              */
/*----------------------------------------------------------------------------*/
void Step::vZeroNotEqual(void)
{
  ENTRY();
  
  i8 iZero = m_pDataStack->iPopAsInt(__func__);
  
  if (iZero != 0)
    m_pDataStack->vPushInt(true);
  else
    m_pDataStack->vPushInt(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* zeroGreaterEqual -- test if top value is greater than or equal to zero     */
/*----------------------------------------------------------------------------*/
void Step::vZeroGreaterEqual(void)
{
  ENTRY();
  
  i8 iZero = m_pDataStack->iPopAsInt(__func__);
  
  if (iZero >= 0)
    m_pDataStack->vPushInt(true);
  else
    m_pDataStack->vPushInt(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 0> -- test if top value is greater than zero                              */
/*----------------------------------------------------------------------------*/
void Step::vZeroGreater(void)
{
  ENTRY();
  
  i8 iZero = m_pDataStack->iPopAsInt(__func__);
  
  if (iZero > 0)
    m_pDataStack->vPushInt(true);
  else
    m_pDataStack->vPushInt(false);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
