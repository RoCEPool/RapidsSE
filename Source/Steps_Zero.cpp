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
void Step::vZeroLess(Forth* pForth)
{
  ENTRY("vZeroLess"_Stz);
  
  i8 iZero;
  if ( ! pForth->m_pDataStack->bPopAsInt(iZero) )
    THROW_DSTACK();
  
  if (iZero < 0)
    pForth->m_pDataStack->vPushBool(true);
  else
    pForth->m_pDataStack->vPushBool(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 0<= -- test if top value is less than or equal to zero                     */
/*----------------------------------------------------------------------------*/
void Step::vZeroLessEqual(Forth* pForth)
{
  ENTRY("vZeroLessEqual"_Stz);
  
  i8 iZero;
  if ( ! pForth->m_pDataStack->bPopAsInt(iZero) )
    THROW_DSTACK();
  
  if (iZero <= 0)
    pForth->m_pDataStack->vPushBool(true);
  else
    pForth->m_pDataStack->vPushBool(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 0== -- test if top value is equal to zero / nullptr                        */
/*----------------------------------------------------------------------------*/
void Step::vZeroEqual(Forth* pForth)
{
  ENTRY("vZeroEqual"_Stz);
  
  i8 iZero;
  if ( ! pForth->m_pDataStack->bPopAsInt(iZero) )
    THROW_DSTACK();
  
  if (iZero == 0)
    pForth->m_pDataStack->vPushBool(true);
  else
    pForth->m_pDataStack->vPushBool(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 0!= -- test if top value is not equal to zero                              */
/*----------------------------------------------------------------------------*/
void Step::vZeroNotEqual(Forth* pForth)
{
  ENTRY("vZeroNotEqual"_Stz);
  
  i8 iZero;
  if ( ! pForth->m_pDataStack->bPopAsInt(iZero) )
    THROW_DSTACK();
  
  if (iZero != 0)
    pForth->m_pDataStack->vPushBool(true);
  else
    pForth->m_pDataStack->vPushBool(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* zeroGreaterEqual -- test if top value is greater than or equal to zero     */
/*----------------------------------------------------------------------------*/
void Step::vZeroGreaterEqual(Forth* pForth)
{
  ENTRY("vZeroGreaterEqual"_Stz);
  
  i8 iZero;
  if ( ! pForth->m_pDataStack->bPopAsInt(iZero) )
    THROW_DSTACK();
  
  if (iZero >= 0)
    pForth->m_pDataStack->vPushBool(true);
  else
    pForth->m_pDataStack->vPushBool(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 0> -- test if top value is greater than zero                              */
/*----------------------------------------------------------------------------*/
void Step::vZeroGreater(Forth* pForth)
{
  ENTRY("vZeroGreater"_Stz);
  
  i8 iZero;
  if ( ! pForth->m_pDataStack->bPopAsInt(iZero) )
    THROW_DSTACK();
  
  if (iZero > 0)
    pForth->m_pDataStack->vPushBool(true);
  else
    pForth->m_pDataStack->vPushBool(false);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
