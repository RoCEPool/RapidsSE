/*============================================================================*/
/* Steps_Logic.cpp - logic step group                                         */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

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
void Step::vLogicAnd(MP_Task mpTask)
{
  ENTRY("vLogicAnd"_Stz);
  
  i8 iArgB;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iArgB) )
    THROW_INVALID_DSTACK("and"_Stz);
  
  i8 iArgA;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iArgA) )
    THROW_INVALID_DSTACK("and"_Stz);
  
  i8 iArgR = (iArgA && iArgB) ? 1 : 0;
  
  mpTask->m_pDataStack->vPushI8(iArgR);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* not -- not of top value                                                    */
/*----------------------------------------------------------------------------*/
void Step::vLogicNot(MP_Task mpTask)
{
  ENTRY("vLogicNot"_Stz);
  
  i8 iValA;
  if ( ! mpTask->m_pDataStack->bPopAsI8_Inline(iValA) )
    THROW_INVALID_DSTACK("not"_Stz);
  
  i8 iNotA = iValA ? 0 : 1;
  
  mpTask->m_pDataStack->vPushI8(iNotA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* or -- logical or of top two values                                         */
/*----------------------------------------------------------------------------*/
void Step::vLogicOr(MP_Task mpTask)
{
  ENTRY("vLogicOr"_Stz);
  
  i8 iArgB;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iArgB) )
    THROW_INVALID_DSTACK("or"_Stz);
  
  i8 iArgA;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iArgA) )
    THROW_INVALID_DSTACK("or"_Stz);
  
  i8 iArgR = (iArgA || iArgB) ? 1 : 0;
  
  mpTask->m_pDataStack->vPushI8(iArgR);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
