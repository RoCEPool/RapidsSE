/*============================================================================*/
/* Steps_Logic.cpp - logic step group                                         */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Logic(void)
{
  vAddStep("and"_Stz,             &Step::vLogicAnd);
  vAddStep("not"_Stz,             &Step::vLogicNot);
  vAddStep("or"_Stz,              &Step::vLogicOr);
}

/*----------------------------------------------------------------------------*/
/* and -- logical and of top two values                                       */
/*----------------------------------------------------------------------------*/
void Step::vLogicAnd(void)
{
  ENTRY();
  
  i8 iArgB = m_pDataStack->iPopAsInt(__func__);
  
  i8 iArgA = m_pDataStack->iPopAsInt(__func__);
  
  i8 iArgR = (iArgA && iArgB) ? 1 : 0;
  
  m_pDataStack->vPushInt(iArgR);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* not -- not of top value                                                    */
/*----------------------------------------------------------------------------*/
void Step::vLogicNot(void)
{
  ENTRY();
  
  i8 iValA = m_pDataStack->iPopAsInt(__func__);
  
  i8 iNotA = iValA ? 0 : 1;
  
  m_pDataStack->vPushInt(iNotA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* or -- logical or of top two values                                         */
/*----------------------------------------------------------------------------*/
void Step::vLogicOr(void)
{
  ENTRY();
  
  i8 iArgB = m_pDataStack->iPopAsInt(__func__);
  
  i8 iArgA = m_pDataStack->iPopAsInt(__func__);
  
  i8 iArgR = (iArgA || iArgB) ? 1 : 0;
  
  m_pDataStack->vPushInt(iArgR);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
