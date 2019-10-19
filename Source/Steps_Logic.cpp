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
void Step::vLogicAnd(Forth* pForth)
{
  ENTRY("vLogicAnd"_Stz);
  
  i8 iArgB;
  if ( ! pForth->m_pDataStack->bPopAsInt(iArgB) )
    THROW_DSTACK();
  
  i8 iArgA;
  if ( ! pForth->m_pDataStack->bPopAsInt(iArgA) )
    THROW_DSTACK();
  
  i8 iArgR = (iArgA && iArgB) ? 1 : 0;
  
  pForth->m_pDataStack->vPushI8(iArgR);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* not -- not of top value                                                    */
/*----------------------------------------------------------------------------*/
void Step::vLogicNot(Forth* pForth)
{
  ENTRY("vLogicNot"_Stz);
  
  i8 iValA;
  if ( ! pForth->m_pDataStack->bPopAsI8_Inline(iValA) )
    THROW_DSTACK();
  
  i8 iNotA = iValA ? 0 : 1;
  
  pForth->m_pDataStack->vPushI8(iNotA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* or -- logical or of top two values                                         */
/*----------------------------------------------------------------------------*/
void Step::vLogicOr(Forth* pForth)
{
  ENTRY("vLogicOr"_Stz);
  
  i8 iArgB;
  if ( ! pForth->m_pDataStack->bPopAsInt(iArgB) )
    THROW_DSTACK();
  
  i8 iArgA;
  if ( ! pForth->m_pDataStack->bPopAsInt(iArgA) )
    THROW_DSTACK();
  
  i8 iArgR = (iArgA || iArgB) ? 1 : 0;
  
  pForth->m_pDataStack->vPushI8(iArgR);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
